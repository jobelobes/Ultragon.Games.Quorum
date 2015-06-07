#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CGraphicsDevice::CGraphicsDevice(CGameWindow* window)
{
	this->m_bFullscreen = false;
	this->m_pWindow = window;
	this->m_pWindow->m_pDevice = this;
	this->m_pCurrBuffer = NULL;
	this->m_pGraphicsDevice = NULL;
	this->m_pCurrRenderTarget = NULL;
	this->m_pSwapChain = NULL;
	this->m_pViewport = new D3D10_VIEWPORT();
}
CGraphicsDevice::~CGraphicsDevice()
{
	if(this->m_pGraphicsDevice != NULL)
		this->m_pGraphicsDevice->Release();
	if(this->m_pCurrRenderTarget != NULL)
		this->m_pCurrRenderTarget->Release();
	if(this->m_pSwapChain != NULL)
		this->m_pSwapChain->Release();
}
#pragma endregion

#pragma region Properties
D3D10_VIEWPORT* CGraphicsDevice::Get_Viewport()
{
	return this->m_pViewport;
}

bool CGraphicsDevice::Get_IsFullscreen()
{
	return this->m_bFullscreen;
}

void CGraphicsDevice::Set_VertexBuffer(CVertexBuffer* value)
{
	this->m_pCurrBuffer = value;
}

void CGraphicsDevice::Set_Indices(CIndexBuffer* value)
{
	this->m_pIndexBuffer = value;
}

void CGraphicsDevice::Set_VertexDeclaration(D3D10_INPUT_ELEMENT_DESC* value, int elements)
{
	this->m_pCurrVertexDeclaration = value;
	this->m_iVertexDeclarationElements = elements;
}

void CGraphicsDevice::Set_RenderState(D3D10_RASTERIZER_DESC* value)
{
	this->m_pGraphicsDevice->CreateRasterizerState(value, &this->m_pRenderState);
	this->m_pGraphicsDevice->RSSetState(this->m_pRenderState);
}

ID3D10Device* CGraphicsDevice::Get_GraphicsDevice()
{
	return this->m_pGraphicsDevice;
}
#pragma endregion

#pragma region Methods
HRESULT CGraphicsDevice::Create()
{
	RECT rect = this->m_pWindow->Get_ClientRectangle();
	long width = rect.right - rect.left;
	long height = rect.bottom - rect.top;

	HRESULT hr = this->CreateDevice(width, height);
	if(FAILED(hr))
		return hr;

	hr = this->CreateBackbuffer();
	if(FAILED(hr))
		return hr;

	hr = this->CreateDepthStencil(width, height);
	if(FAILED(hr))
		return hr;

	this->SetViewport(0, 0, width, height, 0.0f, 1.0f);
	this->CreateRenderState();
	this->CreateBlendState();
}

HRESULT CGraphicsDevice::CreateDevice(UINT width, UINT height)
{
	// generate swap chain(double buffer) description 
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = this->m_pWindow->Get_Handle();
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// create the device and swap chain(double buffers)
	HRESULT hr = D3D10CreateDeviceAndSwapChain(
		0, 
		D3D10_DRIVER_TYPE_HARDWARE,
		0,
		0,
		D3D10_SDK_VERSION,
		&sd,
		&this->m_pSwapChain,
		&this->m_pGraphicsDevice);
	if(FAILED(hr))
		return hr;
}

HRESULT CGraphicsDevice::CreateBackbuffer()
{
	ID3D10Texture2D* backbuffer;
	HRESULT hr = this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backbuffer));
	if(FAILED(hr))
		return hr;

	hr = this->m_pGraphicsDevice->CreateRenderTargetView(backbuffer, 0, &this->m_pCurrRenderTarget);
	if(FAILED(hr))
		return hr;

	backbuffer->Release();

	return S_OK;
}

HRESULT CGraphicsDevice::CreateDepthStencil(UINT width, UINT height)
{
	D3D10_TEXTURE2D_DESC td;
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D10_USAGE_DEFAULT;
	td.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	ID3D10Texture2D* mDepthStencilBuffer;
	HRESULT hr = this->m_pGraphicsDevice->CreateTexture2D(&td, 0, &mDepthStencilBuffer);
	if(FAILED(hr))
		return hr;

	hr = this->m_pGraphicsDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &this->m_pDepthStencilView);
	if(FAILED(hr))
		return hr;

	this->m_pGraphicsDevice->OMSetRenderTargets(1, &this->m_pCurrRenderTarget, this->m_pDepthStencilView);
	
	return S_OK;
}

HRESULT CGraphicsDevice::Destroy()
{
	if(this->m_bFullscreen)
		this->m_pSwapChain->SetFullscreenState(false, NULL);
	if(this->m_pGraphicsDevice != NULL)
		this->m_pGraphicsDevice->Release();
	if(this->m_pCurrRenderTarget != NULL)
		this->m_pCurrRenderTarget->Release();
	if(this->m_pSwapChain != NULL)
		this->m_pSwapChain->Release();

	return S_OK;
}


void CGraphicsDevice::SetResolution(UINT width, UINT height)
{
	this->m_pGraphicsDevice->OMSetRenderTargets(0, NULL, NULL);

	this->m_pCurrRenderTarget->Release();
	this->m_pDepthStencilView->Release();

	this->m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	this->CreateBackbuffer();
	this->CreateDepthStencil(width, height);
}

void CGraphicsDevice::SetViewport(UINT x, UINT y, UINT width, UINT height, float minDepth, float maxDepth)
{
	this->m_pViewport->TopLeftX = x;
	this->m_pViewport->TopLeftY = y;
	this->m_pViewport->Width = width;
	this->m_pViewport->Height = height;
	this->m_pViewport->MinDepth = max(minDepth, 0.01f);
	this->m_pViewport->MaxDepth = maxDepth;
	this->m_pGraphicsDevice->RSSetViewports(1, this->m_pViewport);
}

void CGraphicsDevice::CreateRenderState()
{
	D3D10_RASTERIZER_DESC rd;
	rd.CullMode = D3D10_CULL_BACK;
	rd.FillMode = D3D10_FILL_SOLID;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = false;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = false;
	rd.AntialiasedLineEnable = true;
	this->m_pGraphicsDevice->CreateRasterizerState(&rd, &this->m_pRenderState);
}
void CGraphicsDevice::CreateBlendState()
{
	D3D10_BLEND_DESC bd = {0};
	bd.AlphaToCoverageEnable = false;
	bd.BlendEnable[0] = true;
	bd.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	bd.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	bd.BlendOp = D3D10_BLEND_OP_ADD;
	bd.SrcBlendAlpha = D3D10_BLEND_ONE;
	bd.DestBlendAlpha = D3D10_BLEND_ZERO;
	bd.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	bd.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = this->m_pGraphicsDevice->CreateBlendState(&bd, &this->m_pBlendState);
}

void CGraphicsDevice::ToggleFullscreen()
{
	this->m_pSwapChain->SetFullscreenState(!this->m_bFullscreen, NULL);
	this->m_bFullscreen = !this->m_bFullscreen;

	RECT rect = this->m_pWindow->Get_ClientRectangle();
		this->Resize(rect.right - rect.left, rect.bottom - rect.top);
}

void CGraphicsDevice::Resize(UINT width, UINT height)
{
	this->m_pGraphicsDevice->OMSetRenderTargets(0, NULL, NULL);

	this->m_pCurrRenderTarget->Release();
	this->m_pDepthStencilView->Release();

	this->m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	this->CreateBackbuffer();
	this->CreateDepthStencil(width, height);

	this->SetViewport(0,0,width, height, 0.0f, 1.0f);
}

void CGraphicsDevice::Clear(D3DXCOLOR color)
{
	this->m_pGraphicsDevice->ClearRenderTargetView(this->m_pCurrRenderTarget, color);
	this->m_pGraphicsDevice->ClearDepthStencilView(this->m_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
}

void CGraphicsDevice::Present()
{
	float blendFactors[] = {0,0,0,0};

	this->m_pGraphicsDevice->OMSetBlendState(this->m_pBlendState,blendFactors, 0xffffffff);
	this->m_pSwapChain->Present(0,0);
}

void CGraphicsDevice::DrawPrimitives(D3D10_PRIMITIVE_TOPOLOGY primitiveType, int startVertex, int primitiveCount)
{
	UINT stride = this->m_pCurrBuffer->m_iStride;
	UINT offset = 0;

	this->m_pGraphicsDevice->IASetVertexBuffers(0, 1, &this->m_pCurrBuffer->m_iBuffer, &stride , &offset); 
	this->m_pGraphicsDevice->IASetPrimitiveTopology(primitiveType);
	this->m_pGraphicsDevice->RSSetState(this->m_pRenderState);

	switch(primitiveType)
	{
		case D3D10_PRIMITIVE_TOPOLOGY_LINELIST:
		case D3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
		case D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP:
		case D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
			this->m_pGraphicsDevice->Draw(primitiveCount * 2, startVertex);
			break;
			
		case D3D10_PRIMITIVE_TOPOLOGY_POINTLIST:
			this->m_pGraphicsDevice->Draw(primitiveCount, startVertex);
			break;
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
			this->m_pGraphicsDevice->Draw(primitiveCount * 3, startVertex);
			break;
	};
}

void CGraphicsDevice::DrawIndexedPrimitives(D3D10_PRIMITIVE_TOPOLOGY primitiveType, int baseVertex, int minVertexIndex, int numVertices, int startIndex, int primitiveCount)
{
	UINT stride = this->m_pCurrBuffer->m_iStride;
	UINT offset = 0;
	float blendFactors[] = {0,0,0,0};

	this->m_pGraphicsDevice->IASetVertexBuffers(0, 1, &this->m_pCurrBuffer->m_iBuffer, &stride , &offset); 
	this->m_pGraphicsDevice->IASetIndexBuffer(this->m_pIndexBuffer->m_iBuffer, DXGI_FORMAT_R32_UINT, startIndex);
	this->m_pGraphicsDevice->IASetPrimitiveTopology(primitiveType);
	this->m_pGraphicsDevice->RSSetState(this->m_pRenderState);
	this->m_pGraphicsDevice->OMSetBlendState(this->m_pBlendState,blendFactors, 0xffffffff);

	switch(primitiveType)
	{
		case D3D10_PRIMITIVE_TOPOLOGY_LINELIST:
		case D3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
		case D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP:
		case D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
			this->m_pGraphicsDevice->DrawIndexed(primitiveCount * 2, startIndex, baseVertex);
			break;
			
		case D3D10_PRIMITIVE_TOPOLOGY_POINTLIST:
			this->m_pGraphicsDevice->DrawIndexed(primitiveCount, startIndex, baseVertex);
			break;
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		case D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
			this->m_pGraphicsDevice->DrawIndexed(primitiveCount * 3, startIndex, minVertexIndex);
			break;
	};
}
#pragma endregion