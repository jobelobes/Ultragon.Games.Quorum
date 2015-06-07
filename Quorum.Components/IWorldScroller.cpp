/**
 * @description 
 *   Interface for the CWorldBackground that allows the return of the world
 *   offset and ability to scroll the background by a variable amount.  All the parallaxing,
 *   moving the layers at different speeds, is handled by the class that implements this
 *   interface.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

// needed for the services subsystem to identify the interface type
char* IWorldScroller::Get_Type()
{
	return "IWorldScroller";
}