/**
 * @description 
 *   This is a single layer of the parallaxing background;  essentially
 *   this is a list of tiles that will "walk" and "wrap" as the user moves
 *   its offset.  The class can have a dynamic number of tiles and tile size.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CParallaxBackground : public CTiledPlane
{ 
	private:
		#pragma region Variables
		float m_fMoveRate;
		IInputService* m_pInputService;
		static CGameTime* g_pGameTime;
		#pragma endregion

	protected:
		#pragma region Overriden Methods
		virtual void UpdateTileTextures();
		#pragma endregion

	public:
		#pragma region Properties
		float Get_MoveRate();
		void Get_MoveRate(float);
		#pragma endregion

		#pragma region Constructors and Finalizers
		CParallaxBackground(CGame*, D3DXVECTOR2, float);
		CParallaxBackground(CGame*, D3DXVECTOR3, D3DXVECTOR2, float);
		CParallaxBackground(CGame*, D3DXVECTOR2, D3DXVECTOR2, float);
		CParallaxBackground(CGame*, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, float);
		~CParallaxBackground();
		#pragma endregion

		#pragma region Methods
		virtual void Scroll(D3DXVECTOR2);
		virtual void ScrollTo(D3DXVECTOR2);
		void LoadMap(char* path);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(CGameTime*);
		#pragma endregion
};