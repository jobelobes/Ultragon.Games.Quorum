#include "3ngine.Core.h"

enum RotationModes
{
	Pitch,
	Yaw,
	Roll
};

enum MoveModes
{
	Right,
	Left,
	Up,
	Down,
	Forward,
	Backward
};

class CORE_API ICameraService : public IGameService
{
	public:
		#pragma region Properties
		virtual D3DXMATRIX Get_View() = 0;
		virtual D3DXMATRIX Get_Projection() = 0;

		virtual D3DXVECTOR3 Get_Position() = 0;
		virtual D3DXVECTOR3 Get_Up() = 0;
		virtual D3DXVECTOR3 Get_Forward() = 0;
		virtual D3DXVECTOR3 Get_Right() = 0;
		#pragma endregion

		#pragma region IGameService Methods
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Methods
		virtual void LookAt(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3) = 0;
		virtual void Move(MoveModes, float) = 0;
		virtual void Rotate(RotationModes, float) = 0;
		#pragma endregion
};