#include "3ngine.Core.h"

CSpriteBatch::CSpriteBatch(CGame* game)//CGraphicsDevice* m_pDevice)
: CGameComponent(game)
{
	SetDefaultFont();

	if(FAILED(InitSpriteBatch(game->Get_GraphicsDevice())))//m_pDevice)))
	{
		m_bEnabled = false;
	}
	else
	{
		m_pGraphicsDevice = game->Get_GraphicsDevice();//m_pDevice;
		m_bEnabled = true;
	}

	game->Get_Services()->AddService(this);
}

CSpriteBatch::~CSpriteBatch()
{
	m_pSprite = 0;
	m_pSpriteFont = 0;
	m_pGraphicsDevice = 0;
	m_bEnabled = false;
}

void CSpriteBatch::Begin(UINT flags)
{
	if(m_bEnabled)
	{
		m_pSprite->Begin(flags);

		float bFactor[4] = {0, 0, 0, 0};
		m_pGraphicsDevice->m_pGraphicsDevice->OMGetBlendState(&pCurrentBS, fCurrentBlendFactor, &iCurrentSampleMask);
		m_pGraphicsDevice->m_pGraphicsDevice->OMSetBlendState(m_pAlphaBlendState, bFactor, 0xffffffff);
	}
}

void CSpriteBatch::End()
{
	if(m_bEnabled)
	{
		Flush();
		m_pSprite->End();

		m_pGraphicsDevice->m_pGraphicsDevice->OMSetBlendState(pCurrentBS, fCurrentBlendFactor, iCurrentSampleMask);
	}
}

void CSpriteBatch::DrawBuffered(D3DX10_SPRITE *pSprites, UINT cSprites)
{
	if(m_bEnabled)
	{
		m_pSprite->DrawSpritesBuffered(pSprites, cSprites);
	}
}

void CSpriteBatch::DrawBuffered(CSprite2D *pSprites, UINT cSprites)
{
	if(m_bEnabled)
	{
		//D3DX10_SPRITE* pDSprites = new D3DX10_SPRITE[cSprites];
		for(int i = 0; i < cSprites; ++i)
		{
			//pDSprites[i] = pSprites[i].GetSprite();
			m_pSprite->DrawSpritesBuffered(pSprites[i].GetSprite(), 1);
		}
		//m_pSprite->DrawSpritesBuffered(pDSprites, cSprites);
		//delete pDSprites;
	}
}

void CSpriteBatch::DrawImmediate(D3DX10_SPRITE *pSprites, UINT cSprites, UINT cbSprite, UINT flags)
{
	if(m_bEnabled)
	{
		m_pSprite->DrawSpritesImmediate(pSprites, cSprites, cbSprite, flags);
	}
}

void CSpriteBatch::DrawString(LPCTSTR pString, LPRECT pRect, D3DXCOLOR Color)
{
	if(m_bEnabled)
	{
		m_pSpriteFont->DrawText(0, pString, -1, pRect, DT_NOCLIP, Color);
	}
}

void CSpriteBatch::Flush()
{
	if(m_bEnabled)
	{
		m_pSprite->Flush();
	}
}

void CSpriteBatch::SetFont(D3DX10_FONT_DESC font)
{
	if(m_bEnabled)
	{
		m_pSpriteFont = 0;
		if(FAILED(D3DX10CreateFontIndirect(m_pGraphicsDevice->m_pGraphicsDevice, &font, &m_pSpriteFont)))
			D3DX10CreateFontIndirect(m_pGraphicsDevice->m_pGraphicsDevice, &m_defaultFontDescription, &m_pSpriteFont);
	}
}

void CSpriteBatch::SetProjectionTransform(D3DXMATRIX *pProjTransform)
{
	if(m_bEnabled)
	{
		if(FAILED(m_pSprite->SetProjectionTransform(pProjTransform)))//;
			int i = 0;
	}
}

void CSpriteBatch::SetViewTransform(D3DXMATRIX *pViewTransform)
{
	if(m_bEnabled)
	{
		if(FAILED(m_pSprite->SetViewTransform(pViewTransform)))//;
			int i = 0;
	}
}


void CSpriteBatch::SetDefaultFont()
{
	m_defaultFontDescription.Height = 24;
	m_defaultFontDescription.Width = 0;
	m_defaultFontDescription.Weight = 0;
	m_defaultFontDescription.MipLevels = 1;
	m_defaultFontDescription.Italic = false;
	m_defaultFontDescription.CharSet = DEFAULT_CHARSET;
	m_defaultFontDescription.OutputPrecision = OUT_DEFAULT_PRECIS;
	m_defaultFontDescription.Quality = DEFAULT_QUALITY;
	m_defaultFontDescription.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(m_defaultFontDescription.FaceName, "Ariel");
}

HRESULT CSpriteBatch::InitSpriteBatch(CGraphicsDevice* m_pGraphicsDevice)
{
	if(FAILED(D3DX10CreateSprite(m_pGraphicsDevice->m_pGraphicsDevice, 0, &m_pSprite)))
		return S_FALSE;
	if(FAILED(D3DX10CreateFontIndirect(m_pGraphicsDevice->m_pGraphicsDevice, &m_defaultFontDescription, &m_pSpriteFont)))
		return S_FALSE;

	D3DXMATRIX Ortho2D;
	D3DXMatrixOrthoOffCenterLH(&Ortho2D, 0, (float)m_pGraphicsDevice->Get_Viewport()->Width, 0, (float)m_pGraphicsDevice->Get_Viewport()->Height, 0, 1);
	SetProjectionTransform(&Ortho2D);

	D3D10_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D10_BLEND_DESC));
    bd.AlphaToCoverageEnable = FALSE;
    bd.BlendEnable[0] = TRUE;
    bd.SrcBlend = D3D10_BLEND_SRC_ALPHA;
    bd.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
    bd.BlendOp = D3D10_BLEND_OP_ADD;
    bd.SrcBlendAlpha = D3D10_BLEND_ZERO;
    bd.DestBlendAlpha = D3D10_BLEND_ONE;
    bd.BlendOpAlpha = D3D10_BLEND_OP_ADD;
    bd.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

    m_pGraphicsDevice->m_pGraphicsDevice->CreateBlendState(&bd, &m_pAlphaBlendState);
	iCurrentSampleMask = 0;
	pCurrentBS = NULL;

	return S_OK;
}

char* CSpriteBatch::Get_Type()
{
	return "CSpriteBatch";
}