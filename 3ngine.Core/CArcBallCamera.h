#include "3ngine.Core.h"

enum ArcBallCameraMode
{
	Free,
	RollConstrained
};

class CORE_API CArcBallCamera : public CGameComponent, public ICameraService
{
	private:
		#pragma region Static Variables
		static float InputRotateRate;
		#pragma endregion

		#pragma region Variables
		float m_fDistance;
		float m_fInputDistanceRate;
		D3DXMATRIX m_sView;
		D3DXMATRIX m_sProjection;
		D3DXVECTOR2* m_pRotation;
		D3DXVECTOR3 m_sTarget;
		D3DXQUATERNION* m_pOrientation;
		ArcBallCameraMode m_eMode;

		IInputService* m_pInputService;
		#pragma endregion

	protected:
		#pragma region Overriden Methods
		void HandleWindowsInput(IWindowsInputService*, CGameTime*);
		//void HandleXboxInput(IWindowsInputService*, CGameTime*);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CArcBallCamera(CGame*);
		~CArcBallCamera();
		#pragma endregion

		#pragma region Properties
		D3DXVECTOR3 Get_Target();
		void Set_Target(D3DXVECTOR3);

		float Get_Distance();
		void Set_Distance(float);

		void Set_Position(D3DXVECTOR3);

		ArcBallCameraMode Get_ControlMode();
		#pragma endregion

		#pragma region ICameraService Properties
		virtual D3DXMATRIX Get_View();
		virtual D3DXMATRIX Get_Projection();

		virtual D3DXVECTOR3 Get_Position();
		virtual D3DXVECTOR3 Get_Up();
		virtual D3DXVECTOR3 Get_Forward();
		virtual D3DXVECTOR3 Get_Right();
		#pragma endregion

		#pragma region Methods
		void Reset();
		#pragma endregion

		#pragma region ICameraService Methods
		virtual void LookAt(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3);
		virtual void Move(MoveModes, float);
		virtual void Rotate(RotationModes, float);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion
};

