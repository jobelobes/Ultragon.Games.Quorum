#include "3ngine.Core.h"

class CVertexBuffer;
class CIndexBuffer;
class CMesh;

class CORE_API CGraphicsDevice
{
	private: 
		#pragma region Variables
		bool m_bFullscreen;
		ID3D10Device* m_pGraphicsDevice;
		IDXGISwapChain* m_pSwapChain;
		ID3D10RenderTargetView* m_pCurrRenderTarget;
		ID3D10DepthStencilView* m_pDepthStencilView;
		ID3D10RasterizerState* m_pRenderState;
		ID3D10BlendState* m_pBlendState;

		D3D10_VIEWPORT* m_pViewport;
		D3D10_INPUT_ELEMENT_DESC* m_pCurrVertexDeclaration;
		int m_iVertexDeclarationElements;

		CGameWindow* m_pWindow;
		CVertexBuffer* m_pCurrBuffer;
		CIndexBuffer* m_pIndexBuffer;
		#pragma endregion

		#pragma region Constructors and Finalizers
		CGraphicsDevice(CGameWindow*);
		~CGraphicsDevice();
		#pragma endregion

		#pragma region Methods
		HRESULT Create();
		HRESULT Destroy();

		HRESULT CreateDevice(UINT width, UINT height);
		HRESULT CreateBackbuffer();
		HRESULT CreateDepthStencil(UINT width, UINT height);

		void CreateRenderState();
		void CreateBlendState();
		#pragma endregion

	public:
		#pragma region Methods
		void ToggleFullscreen();
		void SetResolution(UINT, UINT);
		void SetViewport(UINT, UINT, UINT, UINT, float, float);
		void Resize(UINT width, UINT height);
		#pragma endregion
		
		#pragma region Properties
		D3D10_VIEWPORT* Get_Viewport();

		bool Get_IsFullscreen();

		void Set_VertexBuffer(CVertexBuffer*);

		void Set_Indices(CIndexBuffer*);

		void Set_VertexDeclaration(D3D10_INPUT_ELEMENT_DESC*, int);

		void Set_RenderState(D3D10_RASTERIZER_DESC*);

		ID3D10Device* Get_GraphicsDevice();
		#pragma endregion

		#pragma region Methods
		void Clear(D3DXCOLOR);

		void Present();

		void DrawPrimitives(D3D10_PRIMITIVE_TOPOLOGY, int, int);

		void DrawIndexedPrimitives(D3D10_PRIMITIVE_TOPOLOGY, int, int, int, int, int);
		#pragma endregion

		friend class CGame;
		friend class CSpriteBatch;
		friend class CVertexBuffer;
		friend class CIndexBuffer;
		friend class CEffect;
		friend class CTexture2D;
		friend class CMesh;
};