#include "3ngine.Core.h"

class CORE_API CTexture2D
{
	private:
		ID3D10ShaderResourceView* m_pShaderResView;
		CGraphicsDevice* m_pGraphicsDevice;

		CTexture2D(CGraphicsDevice*);

	public:
		static CTexture2D* Load(CGraphicsDevice*, char*);

		friend class CSprite2D;
		friend class CEffect;
};