#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CSprite2D::CSprite2D()
{
	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);

	//D3DX10_SPRITE sprite;
	/*m_pSprite.matWorld = Identity;
	m_pSprite.pTexture = NULL;
	m_pSprite.TexCoord.x = 0;
	m_pSprite.TexCoord.y = 0;
	m_pSprite.TexSize.x = 1;
	m_pSprite.TexSize.y = 1;
	m_pSprite.TextureIndex = 0;
	m_pSprite.ColorModulate = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);*/
	//m_pSprite = &sprite;

	m_pSprite = new D3DX10_SPRITE;
	m_pSprite->matWorld = Identity;
	m_pSprite->pTexture = NULL;
	m_pSprite->TexCoord.x = 0;
	m_pSprite->TexCoord.y = 0;
	m_pSprite->TexSize.x = 1;
	m_pSprite->TexSize.y = 1;
	m_pSprite->TextureIndex = 0;
	m_pSprite->ColorModulate = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	
	m_pTexture = NULL;
}

CSprite2D::CSprite2D(D3DX10_SPRITE* sprite)
{
	m_pSprite = sprite;
}

CSprite2D::~CSprite2D()
{
	//m_pSprite = NULL;
}
#pragma endregion

#pragma region Properties
void CSprite2D::SetSprite(D3DX10_SPRITE* sprite)
{
	m_pSprite = sprite;
}

void CSprite2D::SetTransform(D3DXMATRIX matrix)
{
	m_pSprite->matWorld = matrix;
}

void CSprite2D::SetTexCoord(D3DXVECTOR2 texCoord)
{
	m_pSprite->TexCoord = texCoord;
}

void CSprite2D::SetTexSize(D3DXVECTOR2 texSize)
{
	m_pSprite->TexSize = texSize;
}

void CSprite2D::SetColorModulate(D3DXCOLOR colorModulate)
{
	m_pSprite->ColorModulate = colorModulate;
}

/*void CSprite2D::SetTexture(ID3D10ShaderResourceView* texture)
{
	m_pSprite.pTexture = texture;
}*/

void CSprite2D::SetTexture(CTexture2D* texture)
{
	m_pTexture = texture;
	m_pSprite->pTexture = texture->m_pShaderResView;
}

void CSprite2D::SetTextureIndex(UINT textureIndex)
{
	m_pSprite->TextureIndex = textureIndex;
}

D3DX10_SPRITE* CSprite2D::GetSprite()
{
	return m_pSprite;
}

CTexture2D* CSprite2D::GetTexture()
{
	return m_pTexture;
}
#pragma endregion

/*
void CSprite2D::RotateSprite(D3DXVECTOR2 rotation, float rotationAngle)
{
	
}

void CSprite2D::ScaleSprite(D3DXVECTOR2 scale)
{

}

void CSprite2D::Translate(D3DXVECTOR2 translation)
{
	
}

void CSprite2D::TranslateAbs(D3DXVECTOR2 position)
{
	
}

void CSprite2D::Update()
{
	m_pPosition += m_pVelocity;
	D3DXMATRIX t = m_pSprite.matWorld;
	D3DXVECTOR3 pOutScale;
	D3DXQUATERNION pOutRotation;
	D3DXVECTOR3 pOutTranslation;
	D3DXMatrixDecompose(&pOutScale,&pOutRotation,&pOutTranslation,&t);
	
	D3DXMATRIX translate;
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, pOutScale.x, pOutScale.y, pOutScale.z);
	D3DXMatrixTranslation(&translate, m_pPosition.x, m_pPosition.y, m_pPosition.z);
	m_pSprite.matWorld = scale*translate;
}

void CSprite2D::Set_Position(D3DXVECTOR3 position)
{
	m_pPosition = position;
}

void CSprite2D::Set_Velocity(D3DXVECTOR3 velocity)
{
	m_pVelocity = velocity;
}

D3DXVECTOR3 CSprite2D::Get_Velocity()
{
	return m_pVelocity;
}

D3DXVECTOR3 CSprite2D::Get_Position()
{
	return m_pPosition;
}*/
