#include "3ngine.Core.h"

class CORE_API CEffect
{
	private:
		ID3D10Effect* m_pEffect;
		CGraphicsDevice* m_pGraphicsDevice;
		ID3D10EffectMatrixVariable* m_pView;
		ID3D10EffectMatrixVariable* m_pWorld;
		ID3D10EffectMatrixVariable* m_pProjection;
		ID3D10EffectTechnique* m_pCurrTechnique;
		ID3D10InputLayout* m_pInputLayout;

		CEffect(CGraphicsDevice*);
		

	public:
		~CEffect();

		void Begin();
		void End();

		void Set_View(D3DXMATRIX*);
		void Set_World(D3DXMATRIX*);
		void Set_Projection(D3DXMATRIX*);

		ID3D10EffectTechnique* Get_CurrentTechnique();
		void Set_Technique(char*);

		void SetVariable(char*, CTexture2D*);
		void SetVariable(char*, D3DXCOLOR);
		void SetVariable(char*, float);
		void SetVariable(char*, int);
		void SetVariable(char*, D3DXVECTOR2);
		void SetVariable(char*, D3DXVECTOR3);
		void SetVariable(char*, D3DXVECTOR4);

		static CEffect* Load(CGraphicsDevice*, char*);
};