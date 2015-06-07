#include "3ngine.Core.h"

CVertexBuffer::CVertexBuffer(CGraphicsDevice* graphicsDevice, int length, int stride, D3D10_USAGE usage)
{
	this->m_pGraphicsDevice = graphicsDevice;
	this->m_iLength = length;
	this->m_iStride = stride;
	this->m_eBufferUsage = usage;
	this->m_iBuffer = NULL;
}

CVertexBuffer::~CVertexBuffer()
{
	if(this->m_iBuffer != NULL)
		this->m_iBuffer->Release();
}

void CVertexBuffer::CreateBuffer(void* data)
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = this->m_eBufferUsage;
	bd.ByteWidth = this->Get_SizeInBytes();
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = data;

	this->m_pGraphicsDevice->m_pGraphicsDevice->CreateBuffer(&bd, &vbData, &this->m_iBuffer);
}


void CVertexBuffer::SetData(void* buffer)
{
	this->CreateBuffer(buffer);
}

int CVertexBuffer::Get_SizeInBytes()
{
	return this->m_iLength * this->m_iStride;
}