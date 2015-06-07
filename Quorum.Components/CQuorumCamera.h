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

class QUORUM_COMPONENTS_API CQuorumCamera : public CTopDownCamera
{
	private:
		#pragma region Constructors and Finalizers
		static float InputTurnRate;
		static float InputMoveRate;
		static float InputZoomRate;
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CQuorumCamera(CGame*, float);
		~CQuorumCamera();
		#pragma endregion
};

