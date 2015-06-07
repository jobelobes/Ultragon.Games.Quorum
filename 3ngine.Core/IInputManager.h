#include "3ngine.Core.h"

enum Platform
{
	Windows,
	Xbox360
};

enum PlayerIndex
{
	One,
	Two,
	Three,
	Four
};

class IInputService;

class CORE_API IInputManager : public IGameService
{
	public:
		#pragma region Properties
		virtual char* Get_Type();
		#pragma endregion

        #pragma region Methods
        virtual bool IsConnected(PlayerIndex) = 0;
		virtual bool IsConnected(Platform, PlayerIndex) = 0;
        virtual IInputService* GetInput(PlayerIndex) = 0;
		virtual IInputService* GetInput(Platform, PlayerIndex) = 0;
        #pragma endregion
};