#include "3ngine.Core.h"

class CORE_API CAnimation : public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
		static ICameraService* g_pCamera;
		#pragma endregion

		#pragma region Variables
		int m_iInitialFrame;
		float m_fElapsedTime;
		float m_fFramesPerSecond;
		int m_iFrame;
		int m_iNumFrames;
		bool m_bLoop;
		bool m_bRunning;
		bool m_bPaused;
		bool m_bWaitOnFirstFrame;

		D3DXVECTOR2 m_sImageSize;
		D3DXVECTOR2 m_sTextureSize;
		D3DXVECTOR2 m_sImageScale;
		D3DXVECTOR2 m_sPosition;

		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;
		D3DXMATRIX* m_pWorld;

		CTexture2D* m_pTexture;
		#pragma endregion	

		#pragma region Static Methods
		static void BuildMesh(CGraphicsDevice*);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CAnimation(CGame*);
		//CAnimation(CGame*, char* texture, char* effect, D3DXVECTOR2* imageSize, D3DXVECTOR2* textureSize, D3DXVECTOR2* scale, int frames, int fps, int numFrames, bool loop); 
		~CAnimation();
		#pragma endregion

		#pragma region Properties
		D3DXVECTOR2 Get_ImageScale();
		void Set_ImageScale(D3DXVECTOR2);

		D3DXVECTOR2 Get_ImageSize();
		void Set_ImageSize(D3DXVECTOR2);

		D3DXVECTOR2 Get_TextureSize();
		void Set_TextureSize(D3DXVECTOR2);

		D3DXVECTOR2 Get_Position();
		void Set_Position(D3DXVECTOR2);

		CTexture2D* Get_Texture();
		void Set_Texture(CTexture2D*);

		float Get_FramesPerSecond();
		void Set_FramePerSecond(float);

		int Get_NumberOfFrames();
		void Set_NumberOfFrames(int);

		int Get_InitialFrame();
		void Set_InitialFrame(int);

		bool Get_IsLooping();
		void Set_IsLooping(bool);

		int Get_CurrentFrame();
		void Set_CurrentFrame(int);

		void Set_WaitOnFirstFrame(bool);
		void Set_WorldMatrix(D3DXMATRIX*);
		D3DXMATRIX* Get_WorldMatrix();

		bool Get_IsRunning();
		#pragma endregion

		#pragma region Methods
		void Start();
		void Start(bool);
		void Stop();
		void Pause(bool);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(CGameTime*);
		virtual void Draw(CGameTime*);
		#pragma endregion
};