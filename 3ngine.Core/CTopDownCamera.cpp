#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CTopDownCamera::CTopDownCamera(CGame* game)
: CGameComponent(game)
{
	game->Get_Services()->AddService(this);
	this->m_pOrientation = NULL;

	this->Reset();

	this->m_sNormal = D3DXVECTOR3(0,1,0);
	this->m_fDistance = 10;

	this->Rotate(RotationModes::Pitch, D3DX_PI /4);
}

CTopDownCamera::CTopDownCamera(CGame* game, float distance)
: CGameComponent(game)
{
	game->Get_Services()->AddService(this);
	this->m_pOrientation = NULL;

	this->Reset();

	this->m_sNormal = D3DXVECTOR3(0,1,0);
	this->m_fDistance = distance;
	
	this->Rotate(RotationModes::Pitch, D3DX_PI /4);
}

CTopDownCamera::CTopDownCamera(CGame* game, D3DXVECTOR3 normal, float distance)
: CGameComponent(game)
{
	game->Get_Services()->AddService(this);
	this->m_pOrientation = NULL;

	this->Reset();

	this->m_sNormal = normal;
	this->m_fDistance = distance;
	
	this->Rotate(RotationModes::Pitch, D3DX_PI /4);
}

CTopDownCamera::~CTopDownCamera()
{

}
#pragma endregion

#pragma region Properties
D3DXVECTOR3 CTopDownCamera::Get_Target()
{
	return this->m_sTarget;
}

float CTopDownCamera::Get_Distance()
{
	return this->m_fDistance;
}

void CTopDownCamera::Set_Distance(float value)
{
	this->m_fDistance = value;
}

void CTopDownCamera::Set_Position(D3DXVECTOR3 value)
{
	this->LookAt(value, this->m_sTarget, D3DXVECTOR3(0,1,0));
}
#pragma endregion

#pragma region Overriden Properties
D3DXMATRIX CTopDownCamera::Get_View()
{
	D3DXMATRIX view;
	D3DXMatrixLookAtRH(&view, &this->Get_Position(), &this->m_sTarget, &this->Get_Up());
	return view;
}

D3DXMATRIX CTopDownCamera::Get_Projection()
{
	return this->m_sProjection;
}

D3DXVECTOR3 CTopDownCamera::Get_Position()
{
	return this->m_sTarget - (this->Get_Forward() * this->m_fDistance);
}

D3DXVECTOR3 CTopDownCamera::Get_Up()
{
	D3DXVECTOR3 up;
	up.x = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->y) - (this->m_pOrientation->z * this->m_pOrientation->w));
	up.y = ((this->m_pOrientation->y * this->m_pOrientation->y) + (this->m_pOrientation->w * this->m_pOrientation->w)) - 
		((this->m_pOrientation->z * this->m_pOrientation->z) + (this->m_pOrientation->x * this->m_pOrientation->x));
	up.z = 2.0f * ((this->m_pOrientation->y * this->m_pOrientation->z) + (this->m_pOrientation->x * this->m_pOrientation->w));
	D3DXVec3Normalize(&up, &up);
	return up;
}

D3DXVECTOR3 CTopDownCamera::Get_Forward()
{
	D3DXVECTOR3 forward;
	forward.x = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->z) - (this->m_pOrientation->w * this->m_pOrientation->y));
	forward.y = 2.0f * ((this->m_pOrientation->w * this->m_pOrientation->x) - (this->m_pOrientation->y * this->m_pOrientation->z));
	forward.z = ((this->m_pOrientation->x * this->m_pOrientation->x) + (this->m_pOrientation->y * this->m_pOrientation->y)) - 
		((this->m_pOrientation->z * this->m_pOrientation->z) + (this->m_pOrientation->w * this->m_pOrientation->w));
	D3DXVec3Normalize(&forward, &forward);
	return forward;
}

D3DXVECTOR3 CTopDownCamera::Get_Right()
{
	D3DXVECTOR3 right;
	right.x = ((this->m_pOrientation->x * this->m_pOrientation->x) + (this->m_pOrientation->w * this->m_pOrientation->w)) - 
		((this->m_pOrientation->z * this->m_pOrientation->z) + (this->m_pOrientation->y * this->m_pOrientation->y));
	right.y = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->y) - (this->m_pOrientation->z * this->m_pOrientation->w));
	right.z = 2.0f * ((this->m_pOrientation->x * this->m_pOrientation->z) - (this->m_pOrientation->y * this->m_pOrientation->w));
	D3DXVec3Normalize(&right, &right);
	return right;
}
#pragma endregion

#pragma region Methods
void CTopDownCamera::Reset()
{
	if(this->m_pOrientation == NULL)
		this->m_pOrientation = new D3DXQUATERNION();
	D3DXQuaternionRotationAxis(this->m_pOrientation ,&D3DXVECTOR3(0,1,0), 0);
    this->m_sTarget = D3DXVECTOR3(0,0,0);
    this->m_pRotation = &D3DXVECTOR2(0, 0);
}
#pragma endregion

#pragma region Overriden Methods
void CTopDownCamera::LookAt(D3DXVECTOR3 position, D3DXVECTOR3 target, D3DXVECTOR3 up)
{
	this->m_sTarget = target;
	D3DXMATRIX temp;
	D3DXMatrixLookAtRH(&temp, &position, &this->m_sTarget, &up);
	
	D3DXMatrixInverse(&temp, NULL, &temp);
	D3DXQuaternionRotationMatrix(this->m_pOrientation, &temp);

	D3DXVECTOR3* dir = &this->Get_Forward();
	dir->y = 0;
	dir = (D3DXVec3Length(dir) == 0.0f?&D3DXVECTOR3(0,0,-1):dir);
	D3DXVec3Normalize(dir, dir);

	this->m_pRotation->x = acosf(-dir->z)*(dir->x == 0?1:dir->x/abs(dir->x));
	this->m_pRotation->y = -(acosf(D3DXVec3Dot(&D3DXVECTOR3(0,1,0), &this->Get_Forward())) - D3DX_PI / 2.0);
}

void CTopDownCamera::Move(MoveModes mode, float step)
{
	switch (mode)
    {
		case MoveModes::Right:
        case MoveModes::Left:
            {
                D3DXVECTOR3 offset = this->Get_Right() * step;
                this->LookAt(this->Get_Position() - offset, this->Get_Target() - offset, this->Get_Up());
                break;
            }
        case MoveModes::Up:
        case MoveModes::Down:
            {
				D3DXVECTOR3 tmp;
				::D3DXVec3Cross(&tmp, &this->Get_Right(), &this->m_sNormal);
				tmp *= step;
                this->LookAt(this->Get_Position() - tmp, this->Get_Target() - tmp, this->Get_Up());
                break;
            }
        default:
            {
                this->m_fDistance -= 1.0f;
                break;
            }
    }
}

void CTopDownCamera::Rotate(RotationModes mode, float angle)
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
	}
}

void CTopDownCamera::Initialize()
{
	CGameComponent::Initialize();

	D3D10_VIEWPORT* viewport = this->Get_Game()->Get_GraphicsDevice()->Get_Viewport();
	float aspectRatio = (float)viewport->Width / (float)viewport->Height;
	D3DXMatrixPerspectiveFovRH(&this->m_sProjection, D3DX_PI /4, aspectRatio, .1f, 10000.0f);
}

void CTopDownCamera::Update(CGameTime*)
{
	D3D10_VIEWPORT* viewport = this->Get_Game()->Get_GraphicsDevice()->Get_Viewport();
	float aspectRatio = (float)viewport->Width / (float)viewport->Height;
	D3DXMatrixPerspectiveFovRH(&this->m_sProjection, D3DX_PI /4, aspectRatio, .1f, 10000.0f);
}
#pragma endregion