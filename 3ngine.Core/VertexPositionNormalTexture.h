#include "3ngine.Core.h"

class CORE_API VertexPositionNormalTexture
{
	public:
		#pragma region Variables
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DXVECTOR2 TextureCoord;

		static const D3D10_INPUT_ELEMENT_DESC VertexElements[3];
		static const int VertexElementCount;
		#pragma endregion

		#pragma region Constructors and Finalizers
		VertexPositionNormalTexture();
		VertexPositionNormalTexture(float x, float y, float z, float nx, float ny, float nz, float u, float v);
		VertexPositionNormalTexture(D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 texCoord);
		#pragma endregion
};