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

class QUORUM_COMPONENTS_API IWorldScroller : public IGameService
{
	public:
		#pragma region IGameService Methods
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Methods
		virtual void Scroll(D3DXVECTOR2) = 0;
		virtual void ScrollTo(D3DXVECTOR2) = 0;

		virtual D3DXVECTOR2 ToWorldCoord(D3DXVECTOR2) = 0;
		virtual D3DXVECTOR2 ToScreenCoord(D3DXVECTOR2) = 0;
		#pragma endregion
};