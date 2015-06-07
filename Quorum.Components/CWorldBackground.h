/**
 * @description 
 *   Collection of the parallax backgrounds into a single object.  This
 *   class maintains the world offset so that items can draw on the canvas
 *   correctly.  Implements a game service so that other classes can hook
 *   onto the functionality.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */


#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CWorldBackground : public CDrawableGameComponent, public IWorldScroller
{ 
	private:
		#pragma region Variables
		D3DXVECTOR2 m_sOffset;
		float m_fSpeed;
		std::vector<CParallaxBackground*> m_pLayers;
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CWorldBackground(CGame*);
		~CWorldBackground();
		#pragma endregion

		#pragma region Methods
		void AddLayer(CParallaxBackground*);
		CParallaxBackground* GetLayer(int);
		int GetNumberOfLayers();
		#pragma endregion

		#pragma region IWorldScroller Methods
		virtual void Scroll(D3DXVECTOR2);
		virtual void ScrollTo(D3DXVECTOR2);

		virtual D3DXVECTOR2 ToWorldCoord(D3DXVECTOR2);
		virtual D3DXVECTOR2 ToScreenCoord(D3DXVECTOR2);
		#pragma endregion

		#pragma region Overriden Properties
		virtual void Set_Enabled(bool);
		virtual void Set_Visible(bool);
		#pragma endregion
};