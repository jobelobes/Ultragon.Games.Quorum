#include "3ngine.Core.h"

class CORE_API CMaterial
{
	public:
		char* strName[MAX_PATH];
		char* strTexture[MAX_PATH];

		D3DXVECTOR3 Ambient;
		D3DXVECTOR3 Diffuse;
		D3DXVECTOR3 Specular;

		int Shininess;
		float Alpha;

		bool HasSpecular;

		ID3D10ShaderResourceView* pTextureRV10;
		ID3D10EffectTechnique* pTechnique;
};