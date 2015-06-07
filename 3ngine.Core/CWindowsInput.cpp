#include "3ngine.Core.h"

#pragma region Properties
Platform CWindowsInput::Get_Platform()
{
	return Platform::Windows;
}

PlayerIndex CWindowsInput::Get_PlayerIndex()
{
	return this->m_ePlayerIndex;
}

DIMOUSESTATE2 CWindowsInput::Get_MouseState()
{
	return this->m_sMouseState;
}

LPPOINT CWindowsInput::Get_MousePosition()
{
	return this->m_pMousePos;
}
#pragma endregion

#pragma region Constructors and Finalizers
CWindowsInput::CWindowsInput(CGame* game)
:CGameComponent(game)
{
	this->m_ePlayerIndex = PlayerIndex::One;
	this->m_pKeyboardDevice = NULL;
	this->m_pMouseDevice = NULL;
	this->m_pMousePos = new POINT();
	this->m_pWindow = this->Get_Game()->Get_Window();

	ZeroMemory(this->m_pCurKeyBuffer, sizeof(this->m_pCurKeyBuffer));
	ZeroMemory(this->m_pPrevKeyBuffer, sizeof(this->m_pPrevKeyBuffer));

	game->Get_Services()->AddService(this);
}

CWindowsInput::CWindowsInput(CGame* game, PlayerIndex player)
:CGameComponent(game)
{
	this->m_ePlayerIndex = player;
	this->m_pKeyboardDevice = NULL;
	this->m_pMouseDevice = NULL;
	this->m_pMousePos = new POINT();
	this->m_pWindow = this->Get_Game()->Get_Window();

	ZeroMemory(this->m_pCurKeyBuffer, sizeof(this->m_pCurKeyBuffer));
	ZeroMemory(this->m_pPrevKeyBuffer, sizeof(this->m_pPrevKeyBuffer));

	game->Get_Services()->AddService(this);
}

CWindowsInput::~CWindowsInput()
{
	if(this->m_pKeyboardDevice)
	{
		this->m_pKeyboardDevice->Unacquire();
		this->m_pKeyboardDevice->Release();
		this->m_pKeyboardDevice = NULL;
	}

	if(this->m_pMouseDevice)
	{
		this->m_pMouseDevice->Unacquire();
		this->m_pMouseDevice->Release();
		this->m_pMouseDevice = NULL;
	}

	if(this->m_pDirectInputContext)
	{
		this->m_pDirectInputContext->Release();
		this->m_pDirectInputContext = NULL;
	}
}
#pragma endregion

#pragma region Methods
void CWindowsInput::InitializeKeyboard()
{
	this->m_pDirectInputContext->CreateDevice(GUID_SysKeyboard, &this->m_pKeyboardDevice, NULL);
	this->m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	this->m_pKeyboardDevice->SetCooperativeLevel(this->m_pWindow->Get_Handle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	this->m_pKeyboardDevice->Acquire();
}

void CWindowsInput::InitializeMouse()
{
	this->m_pDirectInputContext->CreateDevice(GUID_SysMouse, &this->m_pMouseDevice, NULL);
	this->m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
	this->m_pMouseDevice->SetCooperativeLevel(this->m_pWindow->Get_Handle(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	this->m_pMouseDevice->Acquire();

	::GetCursorPos(this->m_pMousePos);
}

void CWindowsInput::UpdateMouse()
{
	HRESULT hr = this->m_pMouseDevice->GetDeviceState(sizeof(this->m_sMouseState), &this->m_sMouseState);
	if(FAILED(hr) && hr == DIERR_INPUTLOST)
	{
		ZeroMemory(&this->m_sMouseState, sizeof(this->m_sMouseState));

		if(SUCCEEDED(this->m_pMouseDevice->Acquire()))
			this->m_pMouseDevice->GetDeviceState(sizeof(this->m_sMouseState), (void**)&this->m_sMouseState);
		else
			return;
	}
	else if(FAILED(hr) && hr == DIERR_NOTACQUIRED)
	{
		if(SUCCEEDED(this->m_pMouseDevice->Acquire()))
			this->m_pMouseDevice->GetDeviceState(sizeof(this->m_sMouseState), (void**)&this->m_sMouseState);
		else
			return;
	}
	::GetCursorPos(this->m_pMousePos);

	//bound the cursor
	RECT rect = this->m_pWindow->Get_ClientRectangle();
	this->m_pMousePos->x = min(max(this->m_pMousePos->x - rect.left, 0), rect.right - rect.left);
	this->m_pMousePos->y = min(max(this->m_pMousePos->y - rect.top, 0), rect.bottom - rect.top);
}

void swapPointers(char *&a, char *&b)
{
	char *temp = a;
	a = b;
	b = a;
}

void CWindowsInput::UpdateKeyboard()
{
	char* bufferPtr[256];
	memcpy(bufferPtr, this->m_pCurKeyBuffer, 256);
	memcpy(this->m_pPrevKeyBuffer, bufferPtr, 256);

	HRESULT hr = this->m_pKeyboardDevice->GetDeviceState(sizeof(this->m_pCurKeyBuffer), (void**)&this->m_pCurKeyBuffer);
	if(FAILED(hr) && hr == DIERR_INPUTLOST)
	{
		ZeroMemory(this->m_pCurKeyBuffer, sizeof(this->m_pCurKeyBuffer));

		if(SUCCEEDED(this->m_pKeyboardDevice->Acquire()))
			this->m_pKeyboardDevice->GetDeviceState(sizeof(this->m_pCurKeyBuffer), (void**)&this->m_pCurKeyBuffer);
		else
			return;
	}
	else if (FAILED(hr) && hr == DIERR_NOTACQUIRED)
	{
		if(SUCCEEDED(this->m_pKeyboardDevice->Acquire()))
			this->m_pKeyboardDevice->GetDeviceState(sizeof(this->m_pCurKeyBuffer), (void**)&this->m_pCurKeyBuffer);
		else
			return;
	}
}

bool CWindowsInput::IsDown(int key)
{
	return (this->m_pCurKeyBuffer[key] & 0x80);
}

bool CWindowsInput::IsUp(int key)
{
	return !(this->m_pCurKeyBuffer[key] & 0x80);
}

bool CWindowsInput::IsPressed(int key)
{
	return (!(this->m_pCurKeyBuffer[key] & 0x80) && (this->m_pPrevKeyBuffer[key] & 0x80));
}
#pragma endregion

#pragma region Overriden Methods
void CWindowsInput::Initialize()
{
	CGameComponent::Initialize();

	::DirectInput8Create(this->m_pWindow->Get_Instance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->m_pDirectInputContext, NULL);

	this->InitializeKeyboard();
	this->InitializeMouse();
}

void CWindowsInput::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);

	this->UpdateKeyboard();
	this->UpdateMouse();
}
#pragma endregion