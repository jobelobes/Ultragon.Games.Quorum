#include "3ngine.Core.h"

const D3D10_INPUT_ELEMENT_DESC VertexPositionColorTexture::VertexElements[] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

const int VertexPositionColorTexture::VertexElementCount = 3;

#pragma region Constructors and Finalizers
VertexPositionColorTexture::VertexPositionColorTexture()
{
	this->Position.x = 0;
	this->Position.y = 0;
	this->Position.z = 0;
	this->Color = 0xffffffff;
	this->TextureCoord.x = 0;
	this->TextureCoord.y = 0;
}

VertexPositionColorTexture::VertexPositionColorTexture(float x, float y, float z, D3DXCOLOR color, float u, float v)
{
	this->Position.x = x;
	this->Position.y = y;
	this->Position.z = z;
	this->Color = color;
	this->TextureCoord.x = u;
	this->TextureCoord.y = v;
}

VertexPositionColorTexture::VertexPositionColorTexture(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXVECTOR2 texC)
{
	this->Position = pos;
	this->Color = color;
	this->TextureCoord = texC;
}
#pragma endregion
