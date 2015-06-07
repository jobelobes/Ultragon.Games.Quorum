#include "3ngine.Core.h"

class CORE_API CSprite2D
{
public:
	CSprite2D();
	CSprite2D(D3DX10_SPRITE* sprite);
	//CSprite2D(D3DXMATRIX matWorld, D3DXVECTOR2 TexCoord, D3DXVECTOR2 TexSize, D3DXCOLOR ColorModulate, ID3D10ShaderResourceView *pTexture, UINT TextureIndex);
	~CSprite2D();

	#pragma region Properties
	void SetSprite(D3DX10_SPRITE* sprite);
	void SetTransform(D3DXMATRIX matrix);
	void SetTexCoord(D3DXVECTOR2 texCoord);
	void SetTexSize(D3DXVECTOR2 texSize);
	void SetColorModulate(D3DXCOLOR colorModulate);
	//void SetTexture(ID3D10ShaderResourceView* texture);
	void SetTexture(CTexture2D*);
	void SetTextureIndex(UINT textureIndex);
	D3DX10_SPRITE* GetSprite();
	CTexture2D* GetTexture();
	#pragma endregion

	/*
	void RotateSprite(D3DXVECTOR2 rotation, float rotationAngle);
	void ScaleSprite(D3DXVECTOR2 scale);
	void Translate(D3DXVECTOR2 translation);
	void TranslateAbs(D3DXVECTOR2 position);
	
	void Update();
	void Set_Position(D3DXVECTOR3);
	void Set_Velocity(D3DXVECTOR3);
	D3DXVECTOR3 Get_Velocity();
	D3DXVECTOR3 Get_Position();*/

private:
	D3DX10_SPRITE* m_pSprite;
	//D3DXVECTOR3 m_pPosition;
	//D3DXVECTOR3 m_pVelocity;
	CTexture2D* m_pTexture;
	/*D3DXVECTOR2 m_pRotation;
	D3DXVECTOR2 m_pTranslation;
	D3DXVECTOR2 m_pScale;
	float m_pRotationAngle;*/
};