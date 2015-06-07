#include "3ngine.Core.h"

CIndexBuffer::CIndexBuffer(CGraphicsDevice* graphicsDevice, int length, int stride, D3D10_USAGE usage)
{
	this->m_pGraphicsDevice = graphicsDevice;
	this->m_iLength = length;
	this->m_iStride = stride;
	this->m_eBufferUsage = usage;
	this->m_iBuffer = NULL;
}

CIndexBuffer::~CIndexBuffer()
{
	if(this->m_iBuffer != NULL)
		this->m_iBuffer->Release();
}


void CIndexBuffer::CreateBuffer(void* data)
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = this->m_eBufferUsage;
	bd.ByteWidth = this->Get_SizeInBytes();
	bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = data;

	HRESULT hr = this->m_pGraphicsDevice->m_pGraphicsDevice->CreateBuffer(&bd, &vbData, &this->m_iBuffer);
}


void CIndexBuffer::SetData(void* buffer)
{
	this->CreateBuffer(buffer);
}

int CIndexBuffer::Get_SizeInBytes()
{
	return this->m_iLength * this->m_iStride;
}