/**
 * @description 
 *   Interface for the CQuorumMissionManager that allows the return of the mission manager
 *	 and all its components. The service will be used mainly in the initialization of the game
 *	 and the network components
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

// needed for the services subsystem to identify the interface type
char* IMissionManager::Get_Type()
{
	return "IMissionManager";
}