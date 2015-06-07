#include "3ngine.Core.h"

class CORE_API IInputService : public IGameService
{
	public:
		#pragma region Methods
		virtual char* Get_Type();
		virtual Platform Get_Platform() = 0;
        virtual PlayerIndex Get_PlayerIndex() = 0;
		#pragma endregion
};