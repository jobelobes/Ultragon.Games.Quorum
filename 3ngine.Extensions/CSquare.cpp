#include "3ngine.Extensions.h"

#pragma region Static Variables
CMesh* CSquare::g_pMesh = NULL;
CEffect* CSquare::g_pEffect = NULL;
CTexture2D* CSquare::g_pTexture = NULL;
#pragma endregion

#pragma region Static Methods
CMesh* CSquare::BuildMesh(CGraphicsDevice* device)
{
	// create vertices
	VertexPositionColor vertices[4];
	vertices[0].Position.x = -0.5f;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -0.5f;
	vertices[0].Color = D3DXCOLOR(0.8f,1.0f,0.8f,1.0f);

	vertices[1].Position.x = 0.5f;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -0.5f;
	vertices[1].Color = D3DXCOLOR(0.8f,1.0f,0.8f,1.0f);

	vertices[2].Position.x = -0.5f;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = 0.5f;
	vertices[2].Color = D3DXCOLOR(0.8f,1.0f,0.8f,1.0f);

	vertices[3].Position.x = 0.5f;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = 0.5f;
	vertices[3].Color = D3DXCOLOR(0.8f,1.0f,0.8f,1.0f);

	CMesh* mesh = new CMesh(device);
	mesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	mesh->SetIndices((UINT*)indices, 6);

	mesh->SetAttributes(NULL, 0);

	mesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColor::VertexElements, VertexPositionColor::VertexElementCount);
	return mesh;
}

void CSquare::Draw(D3DXCOLOR color, D3DXVECTOR3 min, D3DXVECTOR3 max, CGraphicsDevice* device, ICameraService* camera)
{
	if(CSquare::g_pMesh == NULL)
		CSquare::g_pMesh = CSquare::BuildMesh(device);

	if(CSquare::g_pEffect == NULL)
		CSquare::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\BoundingEffect.fx");

	D3DXMATRIX w, t, s;
	D3DXMatrixIdentity(&w);
	D3DXMatrixTranslation(&t, min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2, min.z + (max.z - min.z) / 2);
	D3DXMatrixScaling(&s, max.x - min.x, max.y - min.y, max.z - min.z);
	D3DXMatrixMultiply(&w, &w, &s);
	D3DXMatrixMultiply(&w, &w, &t);

	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColor.VertexElements, VertexPositionColor::VertexElementCount);

	CSquare::g_pEffect->Set_View(&camera->Get_View());
	CSquare::g_pEffect->Set_World(&w);
	CSquare::g_pEffect->Set_Projection(&camera->Get_Projection());
	CSquare::g_pEffect->SetVariable("Color", color);
	CSquare::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CSquare::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CSquare::g_pMesh->Draw();
		}
	}
	CSquare::g_pEffect->End();
}
#pragma endregion
