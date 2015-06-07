#include "3ngine.Extensions.h"

class EXTENSIONS_API IScene
{
	public:
		#pragma region Properties
		virtual ISceneManager* Get_SceneManager() = 0;
		#pragma endregion

		#pragma region Methods
		virtual void Exit() = 0;
		#pragma endregion
};

