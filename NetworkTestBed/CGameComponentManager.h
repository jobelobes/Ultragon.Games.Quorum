#include "3ngine.Core.h"

class CORE_API CGameComponentManager
{
	private:
		#pragma region Nested Classes
		enum ComponentManagerState{Created, Initialized, LoadedContent, Updated, Drawn, Destroyed};
		#pragma endregion

		#pragma region Variables
		CGame* m_pGame;
		ComponentManagerState m_eState;
		std::vector<CGameComponent*> m_lComponents;
		#pragma endregion

	protected:
		#pragma region Constructors and Finalizers
		CGameComponentManager(CGame*);
		#pragma endregion

		#pragma region Methods
		void Initialize();
		void LoadContent();
		void Update(CGameTime*);
		void Draw(CGameTime*);
		#pragma endregion

	public:
		#pragma region Properties
		int Get_Count();
		#pragma endregion

		#pragma region Methods
		void Clear();
		void Add(CGameComponent*);
		void Remove(CGameComponent*);
		#pragma endregion

		#pragma region Operator Overriden Methods
		CGameComponent* operator[] (const int);  
		#pragma endregion

		friend class CGame;
};