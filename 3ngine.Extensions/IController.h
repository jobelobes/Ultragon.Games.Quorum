#include "3ngine.Extensions.h"

enum ControllerMode{ User, AI, Network, None };

class EXTENSIONS_API IController
{
	public:
		#pragma region Properties
		virtual bool Get_IsBound() = 0;
		
		virtual ControllerMode Get_Mode() = 0;
		virtual void Set_Mode(ControllerMode) = 0;
		#pragma endregion

		#pragma region Methods
		virtual void Bind(IControllableUnit*) = 0;
		virtual void Unbind() = 0;
		#pragma endregion
};