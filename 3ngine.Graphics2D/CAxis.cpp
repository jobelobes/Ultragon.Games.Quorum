#include "3ngine.Graphics2D.h"

#pragma region Constructors and Finalizers
CAxis::CAxis(CGame* game) 
 : CDrawableGameComponent(game)
{
	this->m_pCamera = NULL;
	this->m_pBuffer = NULL;
	this->m_pEffect = NULL;
	this->m_sWorld = new D3DXMATRIX();
	D3DXMatrixIdentity(this->m_sWorld);
}

CAxis::~CAxis()
{
	if(this->m_pBuffer != NULL)
		delete this->m_pBuffer;
}
#pragma endregion

#pragma region Methods
void CAxis::BuildBuffer()
{
	if(this->m_pBuffer != NULL)
	{
		delete this->m_pBuffer;
		this->m_pBuffer = NULL;
	}

	VertexPositionColor vertices[6];
	// x axis
	vertices[0].Position.x = 1;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = 0;
	vertices[0].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

	vertices[1].Position.x = 0;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = 0;
	vertices[1].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

	// y axis
	vertices[2].Position.x = 0;
	vertices[2].Position.y = 1;
	vertices[2].Position.z = 0;
	vertices[2].Color = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);

	vertices[3].Position.x = 0;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = 0;
	vertices[3].Color = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);

	// z axis
	vertices[4].Position.x = 0;
	vertices[4].Position.y = 0;
	vertices[4].Position.z = 1;
	vertices[4].Color = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);

	vertices[5].Position.x = 0;
	vertices[5].Position.y = 0;
	vertices[5].Position.z = 0;
	vertices[5].Color = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);

	this->m_pBuffer = new CVertexBuffer(this->Get_Game()->Get_GraphicsDevice(), 6, sizeof(VertexPositionColor), D3D10_USAGE_IMMUTABLE);
	this->m_pBuffer->SetData(vertices);
}
#pragma endregion

#pragma region Overriden Methods
void CAxis::Initialize()
{
	CDrawableGameComponent::Initialize();

	this->m_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(this->m_pCamera == NULL)
		exit(-1);
}

void CAxis::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	this->m_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultEffect.fx");
	this->BuildBuffer();	
}

void CAxis::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColor::VertexElements, VertexPositionColor::VertexElementCount);
	device->Set_VertexBuffer(this->m_pBuffer);

	
	this->m_pEffect->Set_View(&this->m_pCamera->Get_View());
	
	this->m_pEffect->Set_World(this->m_sWorld);
	this->m_pEffect->Set_Projection(&this->m_pCamera->Get_Projection());
	this->m_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = this->m_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			device->DrawPrimitives(D3D10_PRIMITIVE_TOPOLOGY_LINELIST, 0, 3);
		}
	}
	this->m_pEffect->End();
}
#pragma endregion