#include "3ngine.Core.h"

#pragma region Static Variables
float CArcBallCamera::InputRotateRate = 10.0f;
#pragma endregion

#pragma region Constructors and Finalizers
CArcBallCamera::CArcBallCamera(CGame* game)
: CGameComponent(game)
{
	this->m_fDistance = 10;
	this->m_pOrientation = new D3DXQUATERNION();
	game->Get_Services()->AddService(this);
}

CArcBallCamera::~CArcBallCamera()
{

}
#pragma endregion

#pragma region Properties
D3DXVECTOR3 CArcBallCamera::Get_Target()
{
	return this->m_sTarget;
}

float CArcBallCamera::Get_Distance()
{
	return this->m_fDistance;
}

void CArcBallCamera::Set_Distance(float value)
{
	this->m_fDistance = value;
}

void CArcBallCamera::Set_Position(D3DXVECTOR3 value)
{
	this->LookAt(value, this->m_sTarget, D3DXVECTOR3(0,1,0));
}

ArcBallCameraMode CArcBallCamera::Get_ControlMode()
{
	return this->m_eMode;
}
#pragma endregion

#pragma region Overriden Properties
D3DXMATRIX CArcBallCamera::Get_View()
{
	D3DXMATRIX view;
	D3DXMatrixLookAtRH(&view, &this->Get_Position(), &this->m_sTarget, &this->Get_Up());
	return view;
}

D3DXMATRIX CArcBallCamera::Get_Projection()
{
	return this->m_sProjection;
}

D3DXVECTOR3 CArcBallCamera::Get_Position()
{
	return this->m_sTarget - (this->Get_Forward() * this->m_fDistance);
}

D3DXVECTOR3 CArcBallCamera::Get_Up()
{
	D3DXVECTOR3 up;
	up.x = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->y) - (this->m_pOrientation->z * this->m_pOrientation->w));
	up.y = ((this->m_pOrientation->y * this->m_pOrientation->y) + (this->m_pOrientation->w * this->m_pOrientation->w)) - 
		((this->m_pOrientation->z * this->m_pOrientation->z) + (this->m_pOrientation->x * this->m_pOrientation->x));
	up.z = 2.0f * ((this->m_pOrientation->y * this->m_pOrientation->z) - (this->m_pOrientation->x * this->m_pOrientation->w));
	return up;
}

D3DXVECTOR3 CArcBallCamera::Get_Forward()
{
	D3DXVECTOR3 forward;
	forward.x = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->z) - (this->m_pOrientation->w * this->m_pOrientation->y));
	forward.y = 2.0f * ((this->m_pOrientation->w * this->m_pOrientation->x) - (this->m_pOrientation->y * this->m_pOrientation->z));
	forward.z = ((this->m_pOrientation->x * this->m_pOrientation->x) + (this->m_pOrientation->y * this->m_pOrientation->y)) - 
		((this->m_pOrientation->z * this->m_pOrientation->z) + (this->m_pOrientation->w * this->m_pOrientation->w));
	return forward;
}

D3DXVECTOR3 CArcBallCamera::Get_Right()
{
	D3DXVECTOR3 right;
	right.x = ((this->m_pOrientation->x * this->m_pOrientation->x) + (this->m_pOrientation->w * this->m_pOrientation->w)) - 
		((this->m_pOrientation->z * this->m_pOrientation->z) + (this->m_pOrientation->y * this->m_pOrientation->y));
	right.y = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->y) - (this->m_pOrientation->z * this->m_pOrientation->w));
	right.z = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->z) - (this->m_pOrientation->y * this->m_pOrientation->w));
	return right;
}
#pragma endregion

#pragma region Methods
void CArcBallCamera::Reset()
{
	D3DXQuaternionRotationAxis(this->m_pOrientation, &this->Get_Up(), D3DX_PI);
	this->m_sTarget = D3DXVECTOR3(0,0,0);
	this->m_pRotation = &D3DXVECTOR2(D3DX_PI,0);
	this->LookAt(D3DXVECTOR3(0,10,10), this->m_sTarget, D3DXVECTOR3(0,1,0));
}

void CArcBallCamera::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	float dX = 0;
    float dY = 0;
    float elapsedTime = gameTime->Get_ElapsedTime();

	if (inputService->IsDown(DIK_A) || inputService->IsDown(DIK_D))
		dX = elapsedTime * (inputService->IsDown(DIK_A) ? 1 : -1) * CArcBallCamera::InputRotateRate;
    if (inputService->IsDown(DIK_S) || inputService->IsDown(DIK_W))
        dY = elapsedTime * (inputService->IsDown(DIK_S) ? 1 : -1) * CArcBallCamera::InputRotateRate;

	if (dY != 0) this->Rotate(RotationModes::Pitch, dY);
	if (dX != 0) this->Rotate(RotationModes::Yaw, dX);

    if (inputService->IsDown(DIK_Z) || inputService->IsDown(DIK_X))
    {
		this->m_fDistance += (inputService->IsDown(DIK_Z) ? 1 : -1) * this->m_fInputDistanceRate * elapsedTime;
        this->m_fDistance = max(.001f, this->m_fDistance);
    }

	if (this->m_eMode != ArcBallCameraMode::Free)
    {
        float dR = 0;
        if (inputService->IsDown(DIK_Q) || inputService->IsDown(DIK_E))
            dR = elapsedTime * (inputService->IsDown(DIK_Q) ? 1 : -1) * CArcBallCamera::InputRotateRate;

        if (dR != 0)
			Rotate(RotationModes::Roll, dR);
    }
}

//void CArcBallCamera::HandleXboxInput(IWindowsInputService* inputService, CGameTime* gameTime)
//{
//
//}
#pragma endregion

#pragma region Overriden Methods
void CArcBallCamera::LookAt(D3DXVECTOR3 position, D3DXVECTOR3 target, D3DXVECTOR3 up)
{
	this->m_sTarget = target;
	D3DXMATRIX temp;
	D3DXMatrixLookAtLH(&temp, &position, &this->m_sTarget, &up);
	
	D3DXMatrixInverse(&temp, NULL, &temp);
	D3DXQuaternionRotationMatrix(this->m_pOrientation, &temp);
}

void CArcBallCamera::Move(MoveModes mode, float offset)
{
	//this->Lookat(this->Get_Position() + offset, this->m_sTarget, this->Get_Up());
}

void CArcBallCamera::Rotate(RotationModes mode, float angle)
{
	switch(mode)
	{
		case RotationModes::Pitch:
			{
				D3DXQUATERNION temp;
				D3DXQuaternionRotationAxis(&temp, &D3DXVECTOR3(1,0,0), -angle);
				*this->m_pOrientation *= temp;					
				D3DXQuaternionNormalize(this->m_pOrientation, this->m_pOrientation);
			}
			break;
		case RotationModes::Yaw:
			{
				D3DXQUATERNION temp;
				D3DXQuaternionRotationAxis(&temp, &D3DXVECTOR3(0,1,0), angle);
				*this->m_pOrientation *= temp;					
				D3DXQuaternionNormalize(this->m_pOrientation, this->m_pOrientation);
			}
			break;
		case RotationModes::Roll:
			{
				D3DXQUATERNION temp;
				D3DXQuaternionRotationAxis(&temp, &D3DXVECTOR3(0,0,1), -angle);
				*this->m_pOrientation *= temp;					
				D3DXQuaternionNormalize(this->m_pOrientation, this->m_pOrientation);
			}
			break;
	}
}

void CArcBallCamera::Initialize()
{
	CGameComponent::Initialize();

	IInputManager* inputManager = (IInputManager*)this->Get_Game()->Get_Services()->GetService("IInputManager");
	this->m_pInputService = inputManager->GetInput(PlayerIndex::One);
	if(this->m_pInputService == NULL)
		exit(-1);

	D3DXMatrixLookAtLH(&this->m_sView, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,0,1), &D3DXVECTOR3(0,1,0));

	D3D10_VIEWPORT* viewport = this->Get_Game()->Get_GraphicsDevice()->Get_Viewport();
	float aspectRatio = viewport->Width / viewport->Height;
	D3DXMatrixPerspectiveFovRH(&this->m_sProjection, D3DX_PI /4, aspectRatio, .1f, 10000.0f);
}

void CArcBallCamera::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);

	if(this->m_pInputService->Get_Platform() == Platform::Windows)
		this->HandleWindowsInput((IWindowsInputService*)this->m_pInputService, gameTime);
	//else
	//	this->HandleXboxInput((IWindowsInputService*)this->m_pInputService, gameTime);
}
#pragma endregion