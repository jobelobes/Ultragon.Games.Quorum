#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
XACTEngine::XACTEngine(CGame* game)
:CGameComponent(game)
{
	m_pGlobalSettingsFile = NULL;
	m_pEngine = NULL;
	m_pWaveBank = NULL;
	m_pSoundBank = NULL;
	m_pbWaveBank = NULL;
	m_pbSoundBank = NULL;

	game->Get_Services()->AddService(this);
}

XACTEngine::XACTEngine(CGame* game, char* globalSettingsFilePath)
:CGameComponent(game)
{
	m_pGlobalSettingsFile = globalSettingsFilePath;

	m_pEngine = NULL;
	m_pWaveBank = NULL;
	m_pSoundBank = NULL;
	m_pbWaveBank = NULL;
	m_pbSoundBank = NULL;

	game->Get_Services()->AddService(this);
}

XACTEngine::~XACTEngine()
{
	if(this->m_pEngine)
    {
        this->m_pEngine->ShutDown();
        this->m_pEngine->Release();
    }

    if(m_pbSoundBank)
        delete[] this->m_pbSoundBank;
    this->m_pbSoundBank = NULL;

    if(this->m_pbWaveBank)
        UnmapViewOfFile(this->m_pbWaveBank);
    this->m_pbWaveBank = NULL;

    CoUninitialize();
}
#pragma endregion

#pragma region CGameComponent Methods
void XACTEngine::Initialize()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	XACT3CreateEngine(0, &m_pEngine);

	XACT_RUNTIME_PARAMETERS xrParams;

	if(m_pGlobalSettingsFile != NULL)
	{
		// Load the global settings file and pass it into XACTInitialize
		HANDLE hFile;
		VOID* pGlobalSettingsData = NULL;
		DWORD dwGlobalSettingsFileSize = 0;
		DWORD dwBytesRead;
		bool bSuccess = false;

		hFile = CreateFile(m_pGlobalSettingsFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile)
		{
			dwGlobalSettingsFileSize = GetFileSize(hFile, NULL);
			if(dwGlobalSettingsFileSize != INVALID_FILE_SIZE)
			{
				pGlobalSettingsData = CoTaskMemAlloc(dwGlobalSettingsFileSize);
				if(pGlobalSettingsData)
				{
					if(0 != ReadFile(hFile, pGlobalSettingsData, dwGlobalSettingsFileSize, &dwBytesRead, NULL))
					{
						bSuccess = true;
					}
				}
			}
			CloseHandle(hFile);
		}
		if(!bSuccess)
		{
			if( pGlobalSettingsData )
				CoTaskMemFree( pGlobalSettingsData );
			pGlobalSettingsData = NULL;
			dwGlobalSettingsFileSize = 0;
		}

		// Initialize & create the XACT runtime 
		//XACT_RUNTIME_PARAMETERS xrParams = {0};
		memset(&xrParams, 0, sizeof(xrParams));
		xrParams.pGlobalSettingsBuffer = pGlobalSettingsData;
		xrParams.globalSettingsBufferSize = dwGlobalSettingsFileSize;
		xrParams.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA; // this will tell XACT to delete[] the buffer when its unneeded
		//xrParams.fnNotificationCallback = XACTNotificationCallback;
		xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
		//this->m_pEngine->->Initialize(&xrParams);
	}
	else
	{
		// Initialize & create the XACT runtime 
		//XACT_RUNTIME_PARAMETERS xrParams;
		memset(&xrParams, 0, sizeof(xrParams));
		xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	}

	this->m_pEngine->Initialize(&xrParams);
}

void XACTEngine::Update(CGameTime* gameTime)
{
	if(this->m_pEngine)
		this->m_pEngine->DoWork();
}
#pragma endregion

#pragma region IXACTEngineService Methods
void XACTEngine::PlaySound(XACTINDEX& sound)
{
	if(this->m_pSoundBank)
		this->m_pSoundBank->Play(sound, 0, 0, NULL);
}

void XACTEngine::StopSound(XACTINDEX sound, DWORD flags)
{
	if(this->m_pSoundBank)
		this->m_pSoundBank->Stop(sound, flags);
}

void XACTEngine::PauseEngine(XACTCATEGORY nCategory, bool isPause)
{
	if(this->m_pEngine)
		this->m_pEngine->Pause(nCategory, isPause);
}

void XACTEngine::StopEngine(XACTCATEGORY nCategory, DWORD flags)
{
	if(this->m_pEngine)
		this->m_pEngine->Stop(nCategory, flags);
}

void XACTEngine::GetCueIndex(LPCSTR cue, XACTINDEX* sound)
{
	if(this->m_pSoundBank)
		(*sound) = this->m_pSoundBank->GetCueIndex(cue);
}

void XACTEngine::Set_Volume(XACTCATEGORY nCategory, XACTVOLUME nVolume)
{
	if(this->m_pEngine)
		this->m_pEngine->SetVolume(nCategory, nVolume);
}

//void XACTEngine::Set_Pitch(IXACT3Wave* nWave, XACTPITCH nPitch)
//{
//	if(this->m_pEngine)
//		this->m_pEngine->Set_Pitch(nWave, nPitch);
//}

XACTCATEGORY XACTEngine::Get_Category(PCSTR category)
{
	if(this->m_pEngine)
		return this->m_pEngine->GetCategory(category);

	return -1;
}

void XACTEngine::PlayWave(XACTINDEX wave, DWORD flags, DWORD offset, XACTLOOPCOUNT nLoops, IXACT3Wave** pWave)
{
	if(this->m_pWaveBank)
		this->m_pWaveBank->Play(wave, flags, offset, nLoops, pWave);
}

void XACTEngine::StopWave(XACTINDEX wave, DWORD flags)
{
	if(this->m_pWaveBank)
		this->m_pWaveBank->Stop(wave, flags);
}

void XACTEngine::GetWaveIndex(PCSTR waveName, XACTINDEX* waveIndex)
{
	//if(this->m_pWaveBank)
		(*waveIndex) = this->m_pWaveBank->GetWaveIndex(waveName);
}

void XACTEngine::LoadWaveBank(LPCSTR filename)
{
	DWORD dwFileSize;
    DWORD dwBytesRead;
    HANDLE hMapFile;
    HANDLE hFile;

	// Create an "in memory" XACT wave bank file using memory mapped file IO
    // Memory mapped files tend to be the fastest for most situations assuming you 
    // have enough virtual address space for a full map of the file
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != -1)
		{
			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);
			if(hMapFile)
			{
				this->m_pbWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);    
				if(this->m_pbWaveBank) 
				{
					this->m_pEngine->CreateInMemoryWaveBank(this->m_pbWaveBank, dwFileSize, 0, 0, &m_pWaveBank);
				}                
				CloseHandle(hMapFile); // pbWaveBank maintains a handle on the file so close this unneeded handle
			}
		}
		CloseHandle(hFile); // pbWaveBank maintains a handle on the file so close this unneeded handle
	}
}

void XACTEngine::LoadSoundBank(LPCSTR filename)
{
	DWORD dwFileSize;
    DWORD dwBytesRead;
    HANDLE hMapFile;
    HANDLE hFile;

	// Read and register the sound bank file with XACT.  Do not use memory mapped file IO because the 
    // memory needs to be read/write and the working set of sound banks are small.
    hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        dwFileSize = GetFileSize(hFile, NULL);
        if(dwFileSize != -1)
        {
            // Allocate the data here and free the data when recieving the sound bank destroyed notification
            this->m_pbSoundBank = new BYTE[dwFileSize];
            if(this->m_pbSoundBank) 
            {
                if(0 != ReadFile(hFile, this->m_pbSoundBank, dwFileSize, &dwBytesRead, NULL))
                {
                    this->m_pEngine->CreateSoundBank(this->m_pbSoundBank, dwFileSize, 0, 0, &this->m_pSoundBank);
                }
            }                
        }
        CloseHandle(hFile); 
    }
}
#pragma endregion