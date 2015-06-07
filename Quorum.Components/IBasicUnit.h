/**
 * @description 
 *   Interface for the basic unit in the game.  Provides the basic 
 *   functionality in order to move/control the object in the game via some
 *   type of controller.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API IBasicUnit
{
	public:
		#pragma region Properties
		virtual float Get_MoveRate() = 0;
		virtual float Get_RotateRate() = 0;

		virtual float Get_Rotation() = 0;
		virtual void Set_Rotation(float) = 0;

		virtual D3DXVECTOR2 Get_Position() = 0;
		virtual void Set_Position(D3DXVECTOR2) = 0;
		#pragma endregion

		#pragma region Methods
		virtual void Move(D3DXVECTOR2) = 0;
		virtual void Rotate(float) = 0;
		virtual void Fire(D3DXVECTOR2 dir) = 0;
		#pragma endregion
};