#include "3ngine.Graphics2D.h"

class GRAPHICS2D_API CAxis : public CDrawableGameComponent
{
	private:
		D3DXMATRIX* m_sWorld;
		CVertexBuffer* m_pBuffer;
		CEffect* m_pEffect;
		ICameraService* m_pCamera;

		void BuildBuffer();

	public:
		#pragma region Constructors and Finalizers
		CAxis(CGame*);
		~CAxis();
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Draw(CGameTime*);
		#pragma endregion
};