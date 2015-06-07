#include "3ngine.Core.h"

class CORE_API CTopDownCamera : public CGameComponent, public ICameraService
{
	private:
		#pragma region Variables
		float m_fDistance;
		float m_fInputDistanceRate;

		D3DXMATRIX m_sProjection;

		D3DXVECTOR3 m_sNormal;
		D3DXVECTOR2* m_pRotation;
		D3DXVECTOR3 m_sTarget;
		D3DXQUATERNION* m_pOrientation;
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CTopDownCamera(CGame*);
		CTopDownCamera(CGame*, float);
		CTopDownCamera(CGame*, D3DXVECTOR3, float);
		~CTopDownCamera();
		#pragma endregion

		#pragma region Properties
		D3DXVECTOR3 Get_Target();
		void Set_Target(D3DXVECTOR3);

		float Get_Distance();
		void Set_Distance(float);

		void Set_Position(D3DXVECTOR3);
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

