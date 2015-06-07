#include "3ngine.Core.h"

class CORE_API XACTEngine : public CGameComponent, public IXACTEngineService
{
	private:
		char* m_pGlobalSettingsFile;
		IXACT3Engine* m_pEngine;
		IXACT3WaveBank* m_pWaveBank;
		IXACT3SoundBank* m_pSoundBank;
		VOID* m_pbWaveBank; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
		VOID* m_pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed

	public:
		#pragma region Constructors and Finalizers
		XACTEngine(CGame*);
		XACTEngine(CGame*, char*);
		~XACTEngine();
		#pragma endregion

		#pragma region CGameComponent Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion

		#pragma region IXACTEngineService Methods
		virtual void PlaySound(XACTINDEX&);
		virtual void StopSound(XACTINDEX, DWORD);
		virtual void PauseEngine(XACTCATEGORY, bool);
		virtual void StopEngine(XACTCATEGORY, DWORD);
		virtual void GetCueIndex(LPCSTR, XACTINDEX*);
		virtual void Set_Volume(XACTCATEGORY, XACTVOLUME);
		//virtual void Set_Pitch(IXACT3Wave*, XACTPITCH);
		virtual XACTCATEGORY Get_Category(PCSTR);
		virtual void PlayWave(XACTINDEX, DWORD, DWORD, XACTLOOPCOUNT, IXACT3Wave**);
		virtual void StopWave(XACTINDEX, DWORD);
		virtual void GetWaveIndex(PCSTR, XACTINDEX*);
		virtual void LoadWaveBank(LPCSTR);
		virtual void LoadSoundBank(LPCSTR);
		#pragma endregion
};
