#include "3ngine.Core.h"

class CORE_API CGameComponent
{
	private:
		#pragma region Variables
		CGame* m_pGame;
		bool m_bEnabled;
		#pragma endregion

		#pragma region Methods
		void InternalUpdate(CGameTime*);
		#pragma endregion

	protected:
		#pragma region Constructors and Deconstructors
		CGameComponent(CGame*);
		~CGameComponent();
		#pragma endregion

		#pragma region Properties
		CGame* Get_Game();
		#pragma endregion

	public:
		#pragma region Properties
		virtual bool Get_Enabled();
		virtual void Set_Enabled(bool);		
		#pragma endregion

		#pragma region Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion

		friend class CGameComponentManager;
};

