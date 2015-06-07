#include "3ngine.Extensions.h"

class EXTENSIONS_API IControllableUnit
{
	public:
		#pragma region Properties
		virtual bool Get_IsBound() = 0;
		virtual void Set_IsBound(bool) = 0;
		#pragma endregion
};