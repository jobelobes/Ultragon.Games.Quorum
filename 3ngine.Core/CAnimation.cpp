#include "3ngine.Core.h"

#pragma region Static Variables
CEffect* CAnimation::g_pEffect = NULL;
CMesh* CAnimation::g_pMesh = NULL;
ICameraService* CAnimation::g_pCamera = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
CAnimation::CAnimation(CGame* game) 
 : CDrawableGameComponent(game)
{
	this->m_fElapsedTime = 0;
	this->m_fFramesPerSecond = 10; 
	this->m_iInitialFrame = 0;
	this->m_iFrame = 0;
	this->m_iNumFrames = 15;//0;
	this->m_bLoop = true;//false;
	this->m_bRunning = false;
	this->m_bWaitOnFirstFrame = false;
	this->m_bPaused = false;
	this->m_pTexture = NULL;

	this->m_sPosition.x = 0;
	this->m_sPosition.y = 0;

	this->m_sImageSize.x = 10;
	this->m_sImageSize.y = 10;

	this->m_sTextureSize.x = 10;
	this->m_sTextureSize.y = 10;
	
	this->m_sImageScale.x = 1;
	this->m_sImageScale.y = 1;
	
	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());
}

CAnimation::~CAnimation()
{
	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
}
#pragma endregion

#pragma region Properties
D3DXVECTOR2 CAnimation::Get_ImageScale()
{
	return this->m_sImageScale;
}

void CAnimation::Set_ImageScale(D3DXVECTOR2 value)
{
	memcpy(&this->m_sImageScale, &value, sizeof(D3DXVECTOR2));
}

D3DXVECTOR2 CAnimation::Get_ImageSize()
{
	return this->m_sImageSize;
}

void CAnimation::Set_ImageSize(D3DXVECTOR2 value)
{
	memcpy(&this->m_sImageSize, &value, sizeof(D3DXVECTOR2));
}

D3DXVECTOR2 CAnimation::Get_TextureSize()
{
	return this->m_sTextureSize;
}

void CAnimation::Set_TextureSize(D3DXVECTOR2 value)
{
	memcpy(&this->m_sTextureSize, &value, sizeof(D3DXVECTOR2));
}

D3DXVECTOR2 CAnimation::Get_Position()
{
	return this->m_sPosition;
}

void CAnimation::Set_Position(D3DXVECTOR2 value)
{
	memcpy(&this->m_sPosition, &value, sizeof(D3DXVECTOR2));

	D3DXMatrixTranslation(this->m_pWorld, this->m_sPosition.x, 0, this->m_sPosition.y);
}

float CAnimation::Get_FramesPerSecond()
{
	return this->m_fFramesPerSecond;
}

void CAnimation::Set_FramePerSecond(float fps)
{
	this->m_fFramesPerSecond = fps;
}

int CAnimation::Get_NumberOfFrames()
{
	return this->m_iNumFrames;
}

void CAnimation::Set_NumberOfFrames(int numFrames)
{
	this->m_iNumFrames = numFrames;
}

int CAnimation::Get_InitialFrame()
{
	return this->m_iInitialFrame;
}

void CAnimation::Set_InitialFrame(int initFrame)
{
	this->m_iInitialFrame = initFrame;
}

bool CAnimation::Get_IsLooping()
{
	return this->m_bLoop;
}

void CAnimation::Set_IsLooping(bool looping)
{
	this->m_bLoop = looping;
}

int CAnimation::Get_CurrentFrame()
{
	return this->m_iFrame;
}

void CAnimation::Set_CurrentFrame(int frame)
{
	this->m_iFrame = frame;
}

void CAnimation::Set_WaitOnFirstFrame(bool wait)
{
	this->m_bWaitOnFirstFrame = wait;
}

CTexture2D* CAnimation::Get_Texture()
{
	return this->m_pTexture;
}

void CAnimation::Set_Texture(CTexture2D* texture)
{
	this->m_pTexture = texture;
}

bool CAnimation::Get_IsRunning()
{
	return this->m_bRunning;
}

void CAnimation::Set_WorldMatrix(D3DXMATRIX* world)
{
	this->m_pWorld = world;
}

D3DXMATRIX* CAnimation::Get_WorldMatrix()
{
	return this->m_pWorld;
}
#pragma endregion

#pragma region Methods
void CAnimation::Start()
{
	this->Start(true);
	this->m_bPaused = false;
}

void CAnimation::Start(bool reset)
{
	if(reset)
	{
		this->m_iFrame = m_iInitialFrame;
		this->m_fElapsedTime = 0;
	}
	this->m_bRunning = true;
	this->m_bPaused = false;
}

void CAnimation::Stop()
{
	this->m_bRunning = false;
}

void CAnimation::Pause(bool isPaused)
{
	this->m_bPaused = isPaused;
}
#pragma endregion

#pragma region Overriden Methods
void CAnimation::Initialize()
{
	CDrawableGameComponent::Initialize();

	if(CAnimation::g_pCamera == NULL)
		CAnimation::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
}

void CAnimation::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();

	if(CAnimation::g_pEffect == NULL)
		CAnimation::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\AnimationEffect.fx");

	if(CAnimation::g_pMesh == NULL)
		CAnimation::BuildMesh(device);
}

void CAnimation::Update(CGameTime* gameTime)
{
	CDrawableGameComponent::Update(gameTime);

	if(!this->m_bRunning || this->m_bPaused) return;

	this->m_fElapsedTime += gameTime->Get_ElapsedTime();
	if(this->m_fElapsedTime > (1 / this->m_fFramesPerSecond))
	{
		this->m_iFrame++;
		this->m_fElapsedTime = 0;
		if(this->m_iFrame >= this->m_iNumFrames)
		{
			if(this->m_bLoop)
				m_iFrame = 0;
			else
			{
				if(this->m_bWaitOnFirstFrame)
				{
					m_iFrame = 0;
					this->Pause(true);
				} else {
					this->Set_Enabled(false);
					this->Set_Visible(false);
				}
			}
		}
	}
}

void CAnimation::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	if(!this->m_bRunning) return;

	this->Get_Game()->Get_GraphicsDevice()->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	CAnimation::g_pEffect->Set_View(&CAnimation::g_pCamera->Get_View());
	CAnimation::g_pEffect->Set_World(this->m_pWorld);
	CAnimation::g_pEffect->Set_Projection(&CAnimation::g_pCamera->Get_Projection());

	if(this->m_pTexture != NULL)
		CAnimation::g_pEffect->SetVariable("InputTexture", this->m_pTexture);

	CAnimation::g_pEffect->SetVariable("ImageSize", this->m_sImageSize);
	CAnimation::g_pEffect->SetVariable("TextureSize", this->m_sTextureSize);
	CAnimation::g_pEffect->SetVariable("ImageScale", this->m_sImageScale);
	CAnimation::g_pEffect->SetVariable("Frame", this->m_iFrame);

	CAnimation::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CAnimation::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CAnimation::g_pMesh->Draw();
		}
	}
	CAnimation::g_pEffect->End();

}
#pragma endregion

#pragma region Static Methods
void CAnimation::BuildMesh(CGraphicsDevice* device)
{
	if(CAnimation::g_pMesh != NULL)
		delete CAnimation::g_pMesh;

	CAnimation::g_pMesh = new CMesh(device);
	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -0.5f;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -0.5f;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = 0.5f;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -0.5f;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -0.5f;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = 0.5f;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = 0.5f;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = 0.5f;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	CAnimation::g_pMesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	CAnimation::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CAnimation::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CAnimation::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
}
#pragma endregion