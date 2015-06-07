#include "3ngine.Extensions.h"

class EXTENSIONS_API CCollisionManager : public CGameComponent, public ICollisionService
{
	private:
		#pragma region Static Variables
		static ISceneManager* g_pSceneManager;
		#pragma endregion

		#pragma region Variables
		int m_iGridSize;
		unsigned int* m_pGridItemCount;
		std::vector<ICollidableUnit*>* m_pGrid;
		#pragma endregion

		#pragma region Methods
		ContainmentType CheckCollision(ICollidableUnit*, ICollidableUnit*);

		int ComputeHash(D3DXVECTOR3 pos);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CCollisionManager(CGame*);
		~CCollisionManager();
		#pragma endregion


		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion

		#pragma region ICollisionService Methods
		bool HasCollided(ICollidableUnit*);
		#pragma endregion
};