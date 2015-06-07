#include "3ngine.Graphics2D.h"

class GRAPHICS2D_API CCube : public CDrawableGameComponent
{
	private:
		RECT* m_pRect;
		CVertexBuffer* m_pBuffer;
		CTexture2D* m_pTexture;
		CEffect* m_pEffect;

		ICameraService* m_pCamera;

		void Set_Rectangle(LONG, LONG, LONG, LONG);
		void BuildBuffer();

	public:
		#pragma region Constructors and Finalizers
		CCube(CGame*);
		~CCube();
		#pragma endregion

		#pragma region Properties
		POINT Get_Size();
		void Set_Size(POINT);

		LONG Get_Width();
		void Set_Width(LONG);

		LONG Get_Height();
		void Set_Height(LONG);

		POINT Get_Position();
		void Set_Position(POINT);

		CTexture2D* Get_Texture();
		void Set_Texture(CTexture2D*);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Draw(CGameTime*);
		#pragma endregion
};