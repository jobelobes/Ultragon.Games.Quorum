/**
 * @description 
 *   Derives from the base TopDownCamera but removes the ability for the user
 *   to move it.  Maintains a specific distance from the background plane.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
float CQuorumCamera::InputTurnRate = 30;
float CQuorumCamera::InputMoveRate = 30;
float CQuorumCamera::InputZoomRate = 20;
#pragma endregion

#pragma region Constructors and Finalizers
CQuorumCamera::CQuorumCamera(CGame* game, float distance)
: CTopDownCamera(game, distance)
{
	this->Rotate(RotationModes::Pitch, D3DX_PI / 4);
}

CQuorumCamera::~CQuorumCamera()
{

}
#pragma endregion