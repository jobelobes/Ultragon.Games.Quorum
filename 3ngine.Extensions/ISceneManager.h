#include "3ngine.Extensions.h"

class IScene;

class EXTENSIONS_API ISceneManager : public IGameService
{
	public:
		#pragma region IGameService Methods
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Properties
		virtual IScene* Get_CurrentScene() = 0;
		#pragma endregion

		#pragma region Overriden Methods
		virtual CDrawableGameComponent** GetGameComponents(CBoundingBox*, int*) = 0;
		virtual CGameComponent* Create(int objectId) = 0;
		virtual void Destroy(int objectId, CGameComponent*) = 0;
		#pragma endregion
};

