/**
 * @description 
 *   Draws the basic window structure using a 9-patch texture
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CNinePatch : public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static int g_iReferences;
		static CMesh* g_pMesh;
		static CEffect*	g_pEffect;

		static ICameraService* g_pCamera;
		#pragma endregion

		#pragma region Variables
		D3DXVECTOR2* m_pPosition;
		D3DXVECTOR2* m_pSize;
		CTexture2D* m_pTexture;
		D3DXVECTOR2* m_pTextureSize;
		D3DXMATRIX* m_pWorld;
		#pragma endregion

		#pragma region static Methods
		/**
		 * Builds the static mesh for the Enemy unit class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice*);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CNinePatch(CGame*);
		CNinePatch(CGame*, D3DXVECTOR2, D3DXVECTOR2, CTexture2D*, D3DXVECTOR2);
		~CNinePatch();
		#pragma endregion

		#pragma region Properties
		D3DXVECTOR2 Get_Position();
		void Set_Position(D3DXVECTOR2);

		D3DXVECTOR2 Get_Size();
		void Set_Size(D3DXVECTOR2);

		CTexture2D* Get_Texture();
		void Set_Texture(CTexture2D*, D3DXVECTOR2);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void LoadContent();
		virtual void Draw(CGameTime*);
		#pragma endregion
};

class QUORUM_COMPONENTS_API NinePatchVertex
{
	public:
		#pragma region Variablse
		D3DXVECTOR3 Position;
		D3DXVECTOR2 TextureCoord;
		unsigned int Index;

		static const D3D10_INPUT_ELEMENT_DESC VertexElements[3];
		static const int VertexElementCount;
		#pragma endregion

		#pragma region Constructors and Finalizers
		NinePatchVertex();
		#pragma endregion
};