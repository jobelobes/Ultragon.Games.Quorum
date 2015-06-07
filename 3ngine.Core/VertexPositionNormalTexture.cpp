#include "3ngine.Core.h"

const D3D10_INPUT_ELEMENT_DESC VertexPositionNormalTexture::VertexElements[] = 
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
};

const int VertexPositionNormalTexture::VertexElementCount = 3;

VertexPositionNormalTexture::VertexPositionNormalTexture()
{
	this->Position.x = 0;
	this->Position.y = 0;
	this->Position.z = 0;
	this->Normal.x = 0;
	this->Normal.y = 0;
	this->Normal.z = 0;
	this->TextureCoord.x = 0;
	this->TextureCoord.y = 0;
}

VertexPositionNormalTexture::VertexPositionNormalTexture(float x, float y, float z, float nx, float ny, float nz, float u, float v)
{
	this->Position.x = x;
	this->Position.y = y;
	this->Position.z = z;
	this->Normal.x = nx;
	this->Normal.y = ny;
	this->Normal.z = nz;
	this->TextureCoord.x = u;
	this->TextureCoord.y = v;
}

VertexPositionNormalTexture::VertexPositionNormalTexture(D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 texCoord)
{
	this->Position = position;
	this->Normal = normal;
	this->TextureCoord = texCoord;
}
