#include "Quorum.Components.h"

#pragma region Static Variables
int CFloorHealth::g_iReferences = 0;

CTexture2D* CFloorHealth::g_pTexture = NULL;

CMesh* CFloorHealth::g_pMesh = NULL;

CEffect* CFloorHealth::g_pEffect = NULL;
ICameraService* CFloorHealth::g_pCamera = NULL;

#pragma endregion

#pragma region Constructors and Deconstructors
CFloorHealth::CFloorHealth(CGame* game)
{
	CFloorHealth::g_iReferences++;
}

CFloorHealth::~CFloorHealth()
{
	CFloorHealth::g_iReferences--;

	//If there are no more references to this type of object, de-reference the global variables
	if(CFloorHealth::g_iReferences == 0)
	{
		delete CFloorHealth::g_pMesh;
		CFloorHealth::g_pMesh = NULL;

		delete CFloorHealth::g_pEffect;
		CFloorHealth::g_pEffect = NULL;

		delete CFloorHealth::g_pTexture;
		CFloorHealth::g_pTexture = NULL;
	}
}

#pragma endregion

#pragma region Properties

#pragma endregion

#pragma region Static Methods
//Build the static mesh for the ship
CMesh* CFloorHealth::BuildMesh(CGraphicsDevice* device, float scale)
{
	CMesh* mesh = new CMesh(device);
	
	VertexPositionColorTexture vertices[4];

/*      
 *      X AXIS
 *   -0.5  0  0.5 
 *     ----------
 *      0-----1   |  -1  Z
 *      |    /|   |
 *      |   / |   |     A
 *      |  /  |   |    X
 *      | /   |   |     I
 *      |/    |   |     S
 *      2-----3   |  0
 */

// NUMBER 1
	vertices[0].Position.x = -scale-0.50625f;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -scale;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = scale-0.50625f;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -scale;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -scale-0.50625f;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = scale;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = scale-0.50625f;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = scale;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	mesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	mesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	mesh->SetAttributes((UINT*)attributes, 2);
	mesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	return mesh;
}
#pragma endregion

#pragma region Overriden Methods

//Draw the ship
void CFloorHealth::DrawFloor(CGame* game, CGraphicsDevice* device, D3DXMATRIX* position, int displayNumber)
{

	// If necessary, load the static camera service
	if(CFloorHealth::g_pCamera == NULL)
	{
		CFloorHealth::g_pCamera = (ICameraService*)game->Get_Services()->GetService("ICameraService");
		if(CFloorHealth::g_pCamera == NULL)
			exit(-1);
	}
	
	// If necessary, create the mesh objects
	if(CFloorHealth::g_pMesh == NULL)
		CFloorHealth::g_pMesh = CFloorHealth::BuildMesh(device,2.5f);

	// If necessary, load the effect file
	if(CFloorHealth::g_pEffect == NULL)
		CFloorHealth::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\HudEffect.fx");

	// If necessary, create the texture
	if(CFloorHealth::g_pTexture == NULL)
		CFloorHealth::g_pTexture = CTexture2D::Load(device, "..\\Assets\\Art\\station_floor.png");

	// Initialize the graphics device:
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
	CFloorHealth::g_pEffect->Set_View(&CFloorHealth::g_pCamera->Get_View());	
	CFloorHealth::g_pEffect->Set_Projection(&CFloorHealth::g_pCamera->Get_Projection());
	if(CFloorHealth::g_pTexture != NULL)
			CFloorHealth::g_pEffect->SetVariable("InputTexture", CFloorHealth::g_pTexture);

	CFloorHealth::g_pEffect->SetVariable("StationType", 4);
	
	CFloorHealth::g_pEffect->Set_World(position);

		CFloorHealth::g_pEffect->Begin();
		{
			ID3D10EffectTechnique* technique = CFloorHealth::g_pEffect->Get_CurrentTechnique();

			D3D10_TECHNIQUE_DESC td;
			technique->GetDesc(&td);

			for( UINT p = 0; p < td.Passes; ++p )
			{
				technique->GetPassByIndex(p)->Apply(0);
				CFloorHealth::g_pMesh->Draw();
			}
		}
		CFloorHealth::g_pEffect->End();
}
#pragma endregion