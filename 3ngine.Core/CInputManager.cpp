#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CInputManager::CInputManager(CGame* game)
:CGameComponent(game)
{
	this->m_pInputServices = new IInputService*[4];
	for(int i = 0; i < 4; i++)
		this->m_pInputServices[i] = NULL;

	game->Get_Services()->AddService(this);
}

CInputManager::~CInputManager()
{
	if(this->m_pInputServices != NULL)
		delete [] this->m_pInputServices;
}
#pragma endregion

#pragma region Methods
bool CInputManager::IsConnected(PlayerIndex player)
{
	return (this->GetInput(player) != NULL);
}

bool CInputManager::IsConnected(Platform platform, PlayerIndex player)
{
	return (this->GetInput(platform, player) != NULL);

}

IInputService* CInputManager::GetInput(PlayerIndex player)
{
	if(this->m_pInputServices[(int)player] == NULL)
	{
		IInputService* input = this->GetInput(Platform::Xbox360, player);
		if(input == NULL)
			input = this->GetInput(Platform::Windows, player);
		
		return input;
	}

	return this->m_pInputServices[(int)player];
}

IInputService* CInputManager::GetInput(Platform platform, PlayerIndex player)
{
	if(this->m_pInputServices[(int)player] == NULL)
	{
		IInputService* input = NULL;
		if(platform == Platform::Xbox360 && CXBoxInput::IsControllerConnected(player))
		{
			CXBoxInput* input = new CXBoxInput(this->Get_Game(), player);
			this->m_pInputServices[(int)player] = input;
			this->Get_Game()->Get_Components()->Add(input);
		}
		else if(platform == Platform::Windows && player == PlayerIndex::One)
		{
			CWindowsInput* input = new CWindowsInput(this->Get_Game(), player);
			this->m_pInputServices[(int)player] = input;
			this->Get_Game()->Get_Components()->Add(input);
		}
	}

	return this->m_pInputServices[(int)player];
}
#pragma endregion