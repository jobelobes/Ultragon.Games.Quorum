#include "3ngine.Core.h"

class CORE_API CInputManager : public CGameComponent, public IInputManager
{
	private:
		IInputService** m_pInputServices; 

	public:
		#pragma region Constructors and Finalizers
		CInputManager(CGame*);
		~CInputManager();
		#pragma endregion

        #pragma region Methods
        virtual bool IsConnected(PlayerIndex player);
		virtual bool IsConnected(Platform, PlayerIndex);
        virtual IInputService* GetInput(PlayerIndex player);
		virtual IInputService* GetInput(Platform, PlayerIndex);
        #pragma endregion
};