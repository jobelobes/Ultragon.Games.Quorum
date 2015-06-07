#include "3ngine.Core.h"

class CORE_API VertexPositionColor
{
	public:
		#pragma region Variablse
		D3DXVECTOR3 Position;
		D3DXCOLOR Color;

		static const D3D10_INPUT_ELEMENT_DESC VertexElements[2];
		static const int VertexElementCount;
		#pragma endregion

		#pragma region Constructors and Finalizers
		VertexPositionColor();
		VertexPositionColor(float x, float y, float z, D3DXCOLOR color);
		VertexPositionColor(D3DXVECTOR3 pos, D3DXCOLOR color);
		#pragma endregion
};