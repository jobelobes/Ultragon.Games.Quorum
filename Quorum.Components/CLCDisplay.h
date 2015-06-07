#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CLCDisplay //: public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static int g_iReferences;

		static CMesh* g_pMeshLCD;

		static CTexture2D* g_pTextureLCD;

		static CEffect* g_pEffect;
		static ICameraService* g_pCamera;
		#pragma endregion

		#pragma region Variables

		#pragma endregion
	
	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Constructor that passes in a station to link to
		 *
		 * @param	game	current game object
		 * @param	station	station object that this hud will be linked to
		 */
		CLCDisplay(CGame* game);

		/**
		 * StationHUD deconstructor
		 */
		~CLCDisplay();
		#pragma endregion

		#pragma region Properties

		#pragma endregion

		#pragma region Static Methods
			/**
			 * Builds the static mesh for the LCDisplay class
			 *
			 * @param	device	The graphics device that is registered to the game
			 * @param	scale	The scale used to displace the unit mesh
			 */
			static CMesh* BuildMesh(CGraphicsDevice* device, float scale);

			/**
			 * Builds the static mesh for the Ship class
			 *
			 * @param	game			The current CGame
			 * @param	device			The graphics device
			 * @param	position		The center position of the display
			 * @param	displayNumber	The number to display as xxx%
			 */
			static void DrawLCD(CGame* game, CGraphicsDevice* device, D3DXMATRIX* position, int displayNumber, int stationType);
		#pragma endregion
};