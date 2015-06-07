#include "3ngine.Core.h"

class CORE_API VertexPositionColorTexture
{
	public:
		#pragma region Static Variables
		static const D3D10_INPUT_ELEMENT_DESC VertexElements[3];
		static const int VertexElementCount;
		#pragma endregion

		#pragma region Variables
		D3DXVECTOR3 Position;
		D3DXCOLOR Color;
		D3DXVECTOR2 TextureCoord;
		#pragma endregion

		#pragma region Constructors and Finalizers
		VertexPositionColorTexture();
		VertexPositionColorTexture(float x, float y, float z, D3DXCOLOR color, float u, float v);
		VertexPositionColorTexture(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXVECTOR2 texC);
		#pragma endregion

};