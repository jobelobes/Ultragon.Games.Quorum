/**
 * @description 
 *   Interface for the CScreenManager that allows the ability to manage screens. 
 *	 All the screen management, is handled by the class that implements this nterface.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.Components.h"

#pragma region Overridden Properties
char* IScreenManager::Get_Type()
{
	return "IScreenManager";
}
#pragma endregion
