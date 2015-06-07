#include "Quorum.Components.h"

#pragma region Static Variables
int CLCDisplay::g_iReferences = 0;

CTexture2D* CLCDisplay::g_pTextureLCD = NULL;

CMesh* CLCDisplay::g_pMeshLCD = NULL;

CEffect* CLCDisplay::g_pEffect = NULL;
ICameraService* CLCDisplay::g_pCamera = NULL;

#pragma endregion

#pragma region Constructors and Deconstructors
CLCDisplay::CLCDisplay(CGame* game)
{
	CLCDisplay::g_iReferences++;
}

CLCDisplay::~CLCDisplay()
{
	CLCDisplay::g_iReferences--;

	//If there are no more references to this type of object, de-reference the global variables
	if(CLCDisplay::g_iReferences == 0)
	{
		delete CLCDisplay::g_pMeshLCD;
		CLCDisplay::g_pMeshLCD = NULL;

		delete CLCDisplay::g_pEffect;
		CLCDisplay::g_pEffect = NULL;

		delete CLCDisplay::g_pTextureLCD;
		CLCDisplay::g_pTextureLCD = NULL;
	}
}

#pragma endregion

#pragma region Properties

#pragma endregion

#pragma region Static Methods
//Build the static mesh for the ship
CMesh* CLCDisplay::BuildMesh(CGraphicsDevice* device, float scale)
{
	CMesh* mesh = new CMesh(device);
	
	VertexPositionColorTexture vertices[16];

/*      
 *      X      A       X       I      S
 *     -2     -1       0       1      2
 *     ----------------------------------
 *      0-----1 4-----5 8-----9 12---13  | -1  Z
 *      |    /| |    /| |    /| |    /|  |
 *      |   / | |   / | |   / | |   / |  |     A
 *      |  /  | |  /  | |  /  | |  /  |  |  0  X
 *      | /   | | /   | | /   | | /   |  |     I
 *      |/    | |/    | |/    | |/    |  |     S
 *      2-----3 6-----7 10---11 14---15  |  1
 */

// NUMBER 1
	vertices[0].Position.x = -2*scale;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -scale;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = -scale;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -scale;
	vertices[1].TextureCoord.x = 0.25;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -2*scale;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = scale;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = -scale;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = scale;
	vertices[3].TextureCoord.x = 0.25;
	vertices[3].TextureCoord.y = 1;

// NUMBER 2
	vertices[4].Position.x = -scale;
	vertices[4].Position.y = 0;
	vertices[4].Position.z = -scale;
	vertices[4].TextureCoord.x = 0.25;
	vertices[4].TextureCoord.y = 0;

	vertices[5].Position.x = 0;
	vertices[5].Position.y = 0;
	vertices[5].Position.z = -scale;
	vertices[5].TextureCoord.x = 0.5;
	vertices[5].TextureCoord.y = 0;

	vertices[6].Position.x = -scale;
	vertices[6].Position.y = 0;
	vertices[6].Position.z = scale;
	vertices[6].TextureCoord.x = 0.25;
	vertices[6].TextureCoord.y = 1;

	vertices[7].Position.x = 0;
	vertices[7].Position.y = 0;
	vertices[7].Position.z = scale;
	vertices[7].TextureCoord.x = 0.5;
	vertices[7].TextureCoord.y = 1;

// NUMBER 3
	vertices[8].Position.x = 0;
	vertices[8].Position.y = 0;
	vertices[8].Position.z = -scale;
	vertices[8].TextureCoord.x = 0.5;
	vertices[8].TextureCoord.y = 0;

	vertices[9].Position.x = scale;
	vertices[9].Position.y = 0;
	vertices[9].Position.z = -scale;
	vertices[9].TextureCoord.x = 0.75;
	vertices[9].TextureCoord.y = 0;

	vertices[10].Position.x = 0;
	vertices[10].Position.y = 0;
	vertices[10].Position.z = scale;
	vertices[10].TextureCoord.x = 0.5;
	vertices[10].TextureCoord.y = 1;

	vertices[11].Position.x = scale;
	vertices[11].Position.y = 0;
	vertices[11].Position.z = scale;
	vertices[11].TextureCoord.x = 0.75;
	vertices[11].TextureCoord.y = 1;

// NUMBER 4
	vertices[12].Position.x = scale;
	vertices[12].Position.y = 0;
	vertices[12].Position.z = -scale;
	vertices[12].TextureCoord.x = 0.75;
	vertices[12].TextureCoord.y = 0;

	vertices[13].Position.x = 2*scale;
	vertices[13].Position.y = 0;
	vertices[13].Position.z = -scale;
	vertices[13].TextureCoord.x = 1;
	vertices[13].TextureCoord.y = 0;

	vertices[14].Position.x = scale;
	vertices[14].Position.y = 0;
	vertices[14].Position.z = scale;
	vertices[14].TextureCoord.x = 0.75;
	vertices[14].TextureCoord.y = 1;

	vertices[15].Position.x = 2*scale;
	vertices[15].Position.y = 0;
	vertices[15].Position.z = scale;
	vertices[15].TextureCoord.x = 1;
	vertices[15].TextureCoord.y = 1;

	mesh->SetVertices((void*)vertices, 16);

	UINT indices[24] = {0,1,2,2,1,3, 4,5,6,6,5,7, 8,9,10,10,9,11, 12,13,14,14,13,15};
	mesh->SetIndices((UINT*)indices, 24);

	UINT attributes[8] = {0,0,0,0,0,0,0,0};
	mesh->SetAttributes((UINT*)attributes, 8);
	mesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	return mesh;
}
#pragma endregion

#pragma region Overriden Methods

//Draw the ship
void CLCDisplay::DrawLCD(CGame* game, CGraphicsDevice* device, D3DXMATRIX* position, int displayNumber, int stationType)
{

	// If necessary, load the static camera service
	if(CLCDisplay::g_pCamera == NULL)
	{
		CLCDisplay::g_pCamera = (ICameraService*)game->Get_Services()->GetService("ICameraService");
		if(CLCDisplay::g_pCamera == NULL)
			exit(-1);
	}
	
	// If necessary, create the mesh objects
	if(CLCDisplay::g_pMeshLCD == NULL)
		CLCDisplay::g_pMeshLCD = CLCDisplay::BuildMesh(device,0.5f);

	// If necessary, load the effect file
	if(CLCDisplay::g_pEffect == NULL)
		CLCDisplay::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\LCDTextureEffect.fx");

	// If necessary, create the texture
	if(CLCDisplay::g_pTextureLCD == NULL)
		CLCDisplay::g_pTextureLCD = CTexture2D::Load(device, "..\\Assets\\Art\\numbers.png");

	// Initialize the graphics device:
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
	CLCDisplay::g_pEffect->Set_View(&CLCDisplay::g_pCamera->Get_View());	
	CLCDisplay::g_pEffect->Set_Projection(&CLCDisplay::g_pCamera->Get_Projection());

	if(CLCDisplay::g_pTextureLCD != NULL)
	{
			CLCDisplay::g_pEffect->SetVariable("InputTexture", CLCDisplay::g_pTextureLCD);
			CLCDisplay::g_pEffect->SetVariable("HundredsPlace", int(displayNumber/100)%2);
			CLCDisplay::g_pEffect->SetVariable("TensPlace", int(displayNumber/10)%10);
			CLCDisplay::g_pEffect->SetVariable("OnesPlace", displayNumber%10);
			CLCDisplay::g_pEffect->SetVariable("StationType", stationType);
			// CLCDisplay::g_pEffect->SetVariable("Opacity", CLCDisplay::g_pTextureLCD); // Useful if we want to do any fancy effects on the LCD transparency
	}
	
	CLCDisplay::g_pEffect->Set_World(position);

		CLCDisplay::g_pEffect->Begin();
		{
			ID3D10EffectTechnique* technique = CLCDisplay::g_pEffect->Get_CurrentTechnique();

			D3D10_TECHNIQUE_DESC td;
			technique->GetDesc(&td);

			for( UINT p = 0; p < td.Passes; ++p )
			{
				technique->GetPassByIndex(p)->Apply(0);
				CLCDisplay::g_pMeshLCD->Draw();
			}
		}
		CLCDisplay::g_pEffect->End();
}
#pragma endregion