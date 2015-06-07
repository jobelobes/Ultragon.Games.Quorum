#include "3ngine.Graphics2D.h"

#pragma region Properties
POINT CCube::Get_Size()
{
	POINT pt;
	pt.x = this->m_pRect->right - this->m_pRect->left;
	pt.y = this->m_pRect->bottom - this->m_pRect->top;
	return pt;
}

void CCube::Set_Size(POINT value)
{
	this->Set_Rectangle(this->m_pRect->left, this->m_pRect->top, value.x, value.y);
}

LONG CCube::Get_Width()
{
	return this->m_pRect->right - this->m_pRect->left;
}

void CCube::Set_Width(LONG value)
{
	this->Set_Rectangle(this->m_pRect->left, this->m_pRect->top, value, this->m_pRect->bottom - this->m_pRect->top);
}

LONG CCube::Get_Height()
{
	return this->m_pRect->bottom - this->m_pRect->top;
}

void CCube::Set_Height(LONG value)
{
	this->Set_Rectangle(this->m_pRect->left, this->m_pRect->top, this->m_pRect->right - this->m_pRect->left, value);
}

POINT CCube::Get_Position()
{
	POINT pt;
	pt.x = this->m_pRect->left;
	pt.y = this->m_pRect->top;
	return pt;
}

void CCube::Set_Position(POINT value)
{
	this->Set_Rectangle(value.x, value.y, this->m_pRect->right - this->m_pRect->left, this->m_pRect->bottom - this->m_pRect->top);
}
#pragma endregion

#pragma region Constructors and Finalizers
CCube::CCube(CGame* game) 
 : CDrawableGameComponent(game)
{
	this->m_pCamera = NULL;
	this->m_pBuffer = NULL;
	this->m_pEffect = NULL;
	
	this->m_pRect = new RECT();
	this->Set_Rectangle(0,0,1,1);
}

CCube::~CCube(){}
#pragma endregion

#pragma region Methods
void CCube::Set_Rectangle(LONG x, LONG y, LONG width, LONG height)
{
	this->m_pRect->left = x;
	this->m_pRect->top = y;
	this->m_pRect->right = x + width;
	this->m_pRect->bottom = y + height;
}

void CCube::BuildBuffer()
{
	if(this->m_pBuffer != NULL)
	{
		delete this->m_pBuffer;
		this->m_pBuffer = NULL;
	}

	VertexPositionColor vertices[6];
	//front
	vertices[0].Position.x = (float)this->m_pRect->right;
	vertices[0].Position.y = (float)this->m_pRect->top;
	vertices[0].Position.z = 1;
	vertices[0].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[1].Position.x = (float)this->m_pRect->left;
	vertices[1].Position.y = (float)this->m_pRect->top;
	vertices[1].Position.z = 1;
	vertices[1].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[2].Position.x = (float)this->m_pRect->right;
	vertices[2].Position.y = (float)this->m_pRect->bottom;
	vertices[2].Position.z = 1;
	vertices[2].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[3].Position.x = (float)this->m_pRect->left;
	vertices[3].Position.y = (float)this->m_pRect->top;
	vertices[3].Position.z = 1;
	vertices[3].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[4].Position.x = (float)this->m_pRect->left;
	vertices[4].Position.y = (float)this->m_pRect->bottom;
	vertices[4].Position.z = 1;
	vertices[4].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[5].Position.x = (float)this->m_pRect->right;
	vertices[5].Position.y = (float)this->m_pRect->bottom;
	vertices[5].Position.z = 1;
	vertices[5].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//back
	vertices[0].Position.x = (float)this->m_pRect->right;
	vertices[0].Position.y = (float)this->m_pRect->top;
	vertices[0].Position.z = 1;
	vertices[0].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[1].Position.x = (float)this->m_pRect->left;
	vertices[1].Position.y = (float)this->m_pRect->top;
	vertices[1].Position.z = 1;
	vertices[1].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[2].Position.x = (float)this->m_pRect->right;
	vertices[2].Position.y = (float)this->m_pRect->bottom;
	vertices[2].Position.z = 1;
	vertices[2].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[3].Position.x = (float)this->m_pRect->left;
	vertices[3].Position.y = (float)this->m_pRect->top;
	vertices[3].Position.z = 1;
	vertices[3].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[4].Position.x = (float)this->m_pRect->left;
	vertices[4].Position.y = (float)this->m_pRect->bottom;
	vertices[4].Position.z = 1;
	vertices[4].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vertices[5].Position.x = (float)this->m_pRect->right;
	vertices[5].Position.y = (float)this->m_pRect->bottom;
	vertices[5].Position.z = 1;
	vertices[5].Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	this->m_pBuffer = new CVertexBuffer(this->Get_Game()->Get_GraphicsDevice(), 6, sizeof(VertexPositionColor), D3D10_USAGE_IMMUTABLE);
	this->m_pBuffer->SetData(vertices);
}
#pragma endregion

#pragma region Overriden Methods
void CCube::Initialize()
{
	this->m_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(this->m_pCamera == NULL)
		exit(-1);
}

void CCube::LoadContent()
{
	this->m_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultEffect.fx");

	this->BuildBuffer();	
}

void CCube::Draw(CGameTime*)
{
	D3DXMATRIX w;
	D3DXMatrixIdentity(&w);
	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColor.VertexElements, 2);
	device->Set_VertexBuffer(this->m_pBuffer);

	this->m_pEffect->Set_View(&this->m_pCamera->Get_View());
	this->m_pEffect->Set_World(&w);
	this->m_pEffect->Set_Projection(&this->m_pCamera->Get_Projection());
	this->m_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = this->m_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			device->DrawPrimitives(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 0, 2);
		}
	}
	this->m_pEffect->End();
}
#pragma endregion