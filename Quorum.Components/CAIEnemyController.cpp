/**
 * @description 
 *   Controls the enemy unit; at the moment this controller just determines
 *   a destination and moves towards it, then rechooses another destination.
 *   This will be replaced with a functional AI controller to manage the enemies
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Overriden Properties
ControllerMode CAIEnemyController::Get_Mode()
{
	return ControllerMode::AI;
}
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CAIEnemyController::CAIEnemyController(CGame* game)
:CBaseEnemyController(game)
{
	this->m_eMode = AIMode::Mode0;
	srand(time(NULL));

	this->m_circleAngle = 0.0f;
	this->m_bIsCircling = false;
	this->state = 3;

	ZeroMemory(&this->m_sGoalPosition, sizeof(D3DXVECTOR2));
}


//Parameterized constructor that passes in a controller mode
CAIEnemyController::CAIEnemyController(CGame* game, AIMode mode)
:CBaseEnemyController(game)
{
	this->m_eMode = mode;
	srand(time(NULL));

	this->m_circleAngle = 0.0f;
	this->m_bIsCircling = false;
	this->m_bTowardShip = false;
	this->state = 3;

	ZeroMemory(&this->m_sGoalPosition, sizeof(D3DXVECTOR2));

	this->m_bIsInitial = true;
}

//Deconstructor
CAIEnemyController::~CAIEnemyController(){}
#pragma endregion

#pragma region Methods
void CAIEnemyController::RandomController(CGameTime* gameTime, int identifier)
{
	try
	{
		CEnemyUnit* boundUnit = (CEnemyUnit*)this->Get_BoundObject();
		CShip* ship = boundUnit->Get_Target();

		if(ship != NULL)
		{
			//Update the position of the bound unit based on the control mode
			D3DXVECTOR2 pos = this->Get_BoundObject()->Get_Position();
			if(abs(this->m_sGoalPosition.x - pos.x) < 1 &&
				abs(this->m_sGoalPosition.y - pos.y) < 1)
			{
				this->m_sGoalPosition.x = rand() % 60 - 30;
				this->m_sGoalPosition.y = rand() % 60 - 30;
			}
			else
			{
				float elapsedTime = min(gameTime->Get_ElapsedTime(), 1);

				pos = this->m_sGoalPosition - pos;
				D3DXVec2Normalize(&pos, &pos);
				pos = pos * elapsedTime * this->Get_BoundObject()->Get_MoveRate();
				this->Get_BoundObject()->Move(pos);
			}
				
			D3DXVECTOR2 diff = ship->Get_Position() - boundUnit->Get_Position();
			float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
			D3DXVECTOR2 heading = this->m_sGoalPosition - boundUnit->Get_Position();
			D3DXVec2Normalize(&diff, &diff);
			D3DXVec2Normalize(&heading, &heading);
			float cAngle = D3DXVec2Dot(&heading, &diff);
			float angle = acos(cAngle);

			//Test if we can fire at the ship
			if(angle >= 0.45f && angle <= 1.0f && dist < 35.0f && dist > 10.0f)
			{
				//Fire at Ship
				CEnemyUnit* unit = (CEnemyUnit*)this->Get_BoundObject();
				if(unit->Get_IsOkToFire())
				{
					this->Get_BoundObject()->Fire(diff);

					//Reset the cooldown flag
					unit->Set_IsOkToFire(false);
				}
			}

			CalcRotationAndRotate(&heading);
		}
	}
	catch(char* str)
	{
		//return;
		//int j = 0;
	}
	CGame* g = this->Get_Game();
	this->NetworkUpdate(gameTime, identifier);
}

void CAIEnemyController::CirclingController(CGameTime* gameTime, int identifier)
{
	try
	{
		D3DXVECTOR2 pos = this->Get_BoundObject()->Get_Position();
		CEnemyUnit* boundUnit = (CEnemyUnit*)this->Get_BoundObject();
		CShip* ship = boundUnit->Get_Target();
		D3DXVECTOR2 heading = D3DXVECTOR2(1, 0);
		float elapsedTime = min(gameTime->Get_ElapsedTime(), 1);
		if(ship != NULL)
		{		
			D3DXVECTOR2 diff = ship->Get_Position() - pos;
			float dist = sqrt(diff.x * diff.x + diff.y * diff.y);

			switch(this->state)
			{
			case 0:
				//Don't do anything, ship no longer within range
				m_circleAngle = 0.0f;
				m_bIsCircling = false;

				if(dist < 8)
				{
					this->state = 3;
				}
				else if(dist <= 13)
				{
					this->state = 2;
				}
				else if(dist <= 40)
				{
					this->state = 1;
				}

				break;
			case 1:
				//Move towards ship
				this->m_sGoalPosition = diff;
				D3DXVec2Normalize(&diff, &diff);
				heading = D3DXVECTOR2(diff.x, diff.y);
				diff = diff * elapsedTime * this->Get_BoundObject()->Get_MoveRate();
				this->Get_BoundObject()->Move(diff);

				m_circleAngle = 0.0f;
				m_bIsCircling = false;

				if (dist > 40)
				{
					this->state = 0;
				}
				else if(dist <= 13)
				{
					this->state = 2;
				}
				else if (dist <= 8)
				{
					this->state = 3;
				}

				break;
			case 2:
				//Circle Ship
				if(!m_bIsCircling)
				{
					D3DXVECTOR2 origin = D3DXVECTOR2(1.0f, 0.0f);
					diff = pos - ship->Get_Position();
					D3DXVec2Normalize(&diff, &diff);
					float z = D3DXVec2CCW(&diff, &origin);
					float cAngle = D3DXVec2Dot(&origin, &diff);
					float angle = acos(cAngle);
					if (z < 0) 
					{
						angle = (2.0f * D3DX_PI) - angle;
					}

					m_circleAngle = angle;
					m_bIsCircling = true;
				}

				m_circleAngle += 0.002f;
				if(m_circleAngle > 2*D3DX_PI)
					m_circleAngle -= 2*D3DX_PI;

				heading = D3DXVECTOR2(diff.x, diff.y);
				diff.x = ship->Get_Position().x + dist * cos(m_circleAngle);
				diff.y = ship->Get_Position().y - dist * sin(m_circleAngle);
				diff = diff - pos;
				this->m_sGoalPosition = diff;
				diff = diff * elapsedTime * this->Get_BoundObject()->Get_MoveRate();
				this->Get_BoundObject()->Move(diff);

				//Fire at Ship
				if(boundUnit->Get_IsOkToFire())
				{
					D3DXVECTOR2 target = ship->Get_Position() - pos;
					D3DXVec2Normalize(&target, &target);
					this->Get_BoundObject()->Fire(target);

					//Reset the cooldown flag
					boundUnit->Set_IsOkToFire(false);
				}

				if(dist < 8)
				{
					this->state = 3;
				}
				else if (dist >= 13)
				{
					this->state = 1;
				}
				else if(dist >= 40)
				{
					this->state = 0;
				}

				break;
			case 3:
				//Move Away from ship
				if(diff.x >= -0.1f && diff.x <= 0.1f && diff.y >= -0.1f && diff.y <= 0.1f)
				{
					//Choose a new random direction
					this->m_sGoalPosition.x = rand() % 60 - 30;
					this->m_sGoalPosition.y = rand() % 60 - 30;
				}
				else
				{
					this->m_sGoalPosition = pos - ship->Get_Position();
				}
				diff = this->m_sGoalPosition;
				D3DXVec2Normalize(&diff, &diff);
				heading = D3DXVECTOR2(diff.x, diff.y);
				diff = diff * elapsedTime * this->Get_BoundObject()->Get_MoveRate();
				this->Get_BoundObject()->Move(diff);

				m_circleAngle = 0.0f;
				m_bIsCircling = false;

				if(dist >= 40)
				{
					this->state = 0;
				}
				else if(dist >= 20)
				{
					this->state = 1;
				}

				break;
			}

			CalcRotationAndRotate(&heading);
		}
	}	
	catch(char* str)
	{
		//return;
		//int j = 0;
	}
	CGame* g = this->Get_Game();
	this->NetworkUpdate(gameTime, identifier);
}

void CAIEnemyController::KamikazeController(CGameTime* gameTime, int identifier)
{
	try
	{
		D3DXVECTOR2 pos = this->Get_BoundObject()->Get_Position();
		CEnemyUnit* boundUnit = (CEnemyUnit*)this->Get_BoundObject();
		CShip* ship = boundUnit->Get_Target();
		float elapsedTime = min(gameTime->Get_ElapsedTime(), 1);
		D3DXVECTOR2 steer = D3DXVECTOR2(0, 0);
		D3DXVECTOR2 diff = D3DXVECTOR2(0, 0);
		float dist = 0;
		if(ship != NULL)
		{		
			if(this->m_bIsInitial)
			{
				this->m_sGoalPosition.x = rand() % 60 - 30;
				this->m_sGoalPosition.y = rand() % 60 - 30;
				m_bIsInitial = false;

				this->Get_BoundObject()->Move(this->m_sGoalPosition);
				this->m_sCurrVelocity = ship->Get_Position() - this->Get_BoundObject()->Get_Position();
				D3DXVec2Normalize(&m_sCurrVelocity, &m_sCurrVelocity);
				this->m_sCurrVelocity = this->m_sCurrVelocity * elapsedTime * this->Get_BoundObject()->Get_MoveRate() * 0.002f;
			}
			else
			{
				diff = ship->Get_Position() - pos;
				dist = sqrt(diff.x * diff.x + diff.y * diff.y);
				D3DXVec2Normalize(&diff, &diff);
				steer = diff * (elapsedTime * this->Get_BoundObject()->Get_MoveRate()) * 0.0005f;
			}
			this->m_sCurrVelocity += steer;
			this->Get_BoundObject()->Move(this->m_sCurrVelocity);

			D3DXVECTOR2 heading = this->m_sCurrVelocity;
			D3DXVec2Normalize(&diff, &diff);
			D3DXVec2Normalize(&heading, &heading);
			float cAngle = D3DXVec2Dot(&heading, &diff);
			float angle = acos(cAngle);

			//Test if we can fire at the ship
			if(angle <= 0.80f && angle >= 0.0f && dist < 35.0f && dist > 10.0f)
			{
				//Fire at Ship
				CEnemyUnit* unit = (CEnemyUnit*)this->Get_BoundObject();
				if(unit->Get_IsOkToFire())
				{
					this->Get_BoundObject()->Fire(diff);

					//Reset the cooldown flag
					unit->Set_IsOkToFire(false);
				}
			}

			CalcRotationAndRotate(&heading);		
		}
	}
	catch(char* str)
	{
		//return;
		//int j = 0;
	}
	CGame* g = this->Get_Game();
	this->NetworkUpdate(gameTime, identifier);
}

void CAIEnemyController::CalcRotationAndRotate(D3DXVECTOR2* heading)
{
	D3DXVECTOR2 origin = D3DXVECTOR2(1.0f, 0.0f);
	D3DXVec2Normalize(heading, heading);
	float cAng = D3DXVec2Dot(&origin, heading);
	float ang = acos(cAng);
	float z = D3DXVec2CCW(heading, &origin);
	if (z < 0) 
	{
		ang = (2.0f * D3DX_PI) - ang;
	}

	this->Get_BoundObject()->Rotate(ang + (D3DX_PI / 2.0f));
}
#pragma endregion

#pragma region Overriden Methods
void CAIEnemyController::Initialize()
{
	CBaseEnemyController::Initialize();

	if(this->m_eMode == AIMode::Mode0)
	{
		/*this->m_sGoalPosition.x = rand() % 60 - 30;
		this->m_sGoalPosition.y = rand() % 60 - 30;*/
	}
}

//Update the controller every frame
void CAIEnemyController::Update(CGameTime* gameTime)
{
	//If the bound unit is null or if it isn't enabled, don't bother updating anything
	if(this->Get_BoundObject() == NULL || !((CEnemyUnit*)this->Get_BoundObject())->Get_Enabled())
		return;

	switch(this->m_eMode)
	{
		case AIMode::Mode0:
			this->RandomController(gameTime, ((CEnemyUnit*)this->Get_BoundObject())->Get_Identifier());
			break;
		case AIMode::Mode1:
			this->CirclingController(gameTime, ((CEnemyUnit*)this->Get_BoundObject())->Get_Identifier());
			break;
		case AIMode::Mode2:
			this->KamikazeController(gameTime, ((CEnemyUnit*)this->Get_BoundObject())->Get_Identifier());
			break;
	}
}
#pragma endregion