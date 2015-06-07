#include "3ngine.Core.h"

class CORE_API CDrawableGameComponent : public CGameComponent
{
	private:
		#pragma region Variables
		bool m_bVisible;
		#pragma endregion

		#pragma region Methods
		void InternalDraw(CGameTime*);
		#pragma endregion

	protected:
		#pragma region Constructors and Finalizers
		CDrawableGameComponent(CGame*);
		~CDrawableGameComponent();
		#pragma endregion

	public:
		#pragma region Properties
		virtual bool Get_Visible();
		virtual void Set_Visible(bool);
		#pragma endregion

		#pragma region Methods
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Draw(CGameTime*);
		#pragma endregion

		friend class CGameComponentManager;
};