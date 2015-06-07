#include "3ngine.Core.h"

const D3D10_INPUT_ELEMENT_DESC VertexPositionColor::VertexElements[2] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

const int VertexPositionColor::VertexElementCount = 2;

#pragma region Constructors and Finalizers
VertexPositionColor::VertexPositionColor()
{
	this->Position.x = 0;
	this->Position.y = 0;
	this->Position.z = 0;
	this->Color = 0xffffffff;
}

VertexPositionColor::VertexPositionColor(float x, float y, float z, D3DXCOLOR color)
{
	this->Position.x = x;
	this->Position.y = y;
	this->Position.z = z;
	this->Color = color;
}

VertexPositionColor::VertexPositionColor(D3DXVECTOR3 pos, D3DXCOLOR color)
{
	this->Position = pos;
	this->Color = color;
}
#pragma endregion