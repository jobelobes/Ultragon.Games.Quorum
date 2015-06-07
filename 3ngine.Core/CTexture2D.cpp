#include "3ngine.Core.h"

CTexture2D::CTexture2D(CGraphicsDevice* device)
{
	this->m_pGraphicsDevice = device;
}

CTexture2D* CTexture2D::Load(CGraphicsDevice* device, char* path)
{
	CTexture2D* texture = new CTexture2D(device);

	HRESULT r = D3DX10CreateShaderResourceViewFromFile(
		device->m_pGraphicsDevice, 
		path, 
		0, 
		0, 
		&texture->m_pShaderResView, 
		0);

	return texture;
}