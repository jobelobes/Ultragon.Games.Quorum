#include "3ngine.Extensions.h"

class EXTENSIONS_API CSceneManager : public CDrawableGameComponent, public ISceneManager
{
	public:
		#pragma region Constructors and Finalizers
		CSceneManager(CGame*);
		~CSceneManager();
		#pragma endregion

		#pragma region ISceneManager Properties
		virtual IScene* Get_CurrentScene();
		#pragma endregion

		#pragma region ISceneManager Methods
		virtual CDrawableGameComponent** GetGameComponents(CBoundingBox*, int*);
		#pragma endregion
};

