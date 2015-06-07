#include "3ngine.Core.h"

class CORE_API CSpriteBatch : public CGameComponent, public IGameService
{
public:
	CSpriteBatch(CGame*);//CGraphicsDevice* m_pGraphicsDevice);
	~CSpriteBatch();

	void Begin(UINT flags);
	void End();
	void DrawBuffered(D3DX10_SPRITE *pSprites, UINT cSprites);
	void DrawBuffered(CSprite2D *pSprites, UINT cSprites);
	void DrawImmediate(D3DX10_SPRITE *pSprites, UINT cSprites, UINT cbSprite, UINT flags);
	void DrawString(LPCTSTR pString, LPRECT pRect, D3DXCOLOR Color);
	void Flush();
	void SetFont(D3DX10_FONT_DESC font);
	void SetProjectionTransform(D3DXMATRIX *pProjTransform);
	void SetViewTransform(D3DXMATRIX *pViewTransform);
	void SetDefaultFont();

	#pragma region Overridden Methods
	virtual char* Get_Type();
	#pragma endregion

private:
	//CGame* m_pGame;
	CGraphicsDevice* m_pGraphicsDevice;
	ID3DX10Sprite* m_pSprite;
	ID3DX10Font* m_pSpriteFont;
	bool m_bEnabled;
	D3DX10_FONT_DESC m_defaultFontDescription;
	ID3D10BlendState *m_pAlphaBlendState;

	float fCurrentBlendFactor[4];
	unsigned int iCurrentSampleMask;
	ID3D10BlendState* pCurrentBS;
	
	HRESULT InitSpriteBatch(CGraphicsDevice* m_pGraphicsDevice);
};