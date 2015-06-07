/**
 * @description 
 *   Controls what is drawn for a screen; this class is a base class that all
 *   other screens use to extend how the screen is displayed and how to navigate
 *   to other screens.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.Components.h"

//enum ScreenState
//{
//	Active,
//	Hidden
//};

class QUORUM_COMPONENTS_API CGameScreen : public CDrawableGameComponent//, IScreen
{
	protected:		
		#pragma region Variables
		//bool m_bIsPopup; //Used if screen doesn't cause another to dissappear (potential pause menu, etc.)
		//ScreenState m_eScreenState;
		bool m_bIsActive;
		CSpriteBatch* m_pSpriteBatch;
		CGameScreen* m_pParentScreen;
		#pragma endregion

		#pragma region Methods
		/**
		 * Calls the windows input handler to change the menu selection and select a menu option.
		 *
		 * @param	offset	amount of offset to move the object by
		 */
		virtual void HandleWindowsInput(IWindowsInputService*, CGameTime*);

		/**
		 * Calls the windows xbox handler to change the menu selection and select a menu option.
		 *
		 * @param	offset	amount of offset to move the object by
		 */
		virtual void HandleXboxInput(IXBoxInputService*, CGameTime*);
		#pragma endregion

	public:
		#pragma region Static Variables
		static IInputService* g_pInputService;
		static ICameraService* g_pCameraService;
		static IScreenManager* g_pScreenManager;
		#pragma endregion

		#pragma region Constructors and Finalizers
		CGameScreen(CGame*);
		~CGameScreen();
		#pragma endregion

		#pragma region Methods
		/**
		 * Gets the type of the screen as a string. Base method that all screens will implement.
		 */
		virtual char* Get_Type();

		/**
		 * Activates the screen. Turns on updating and drawing of the screen. Also sets the parent screen so
		 * that we know which screen to return to when going back from this screen. Base method that all screens
		 * will implement.
		 *
		 * @param	parentScreen	screen that called the activate on this screen.
		 */
		virtual void Activate(CGameScreen* parentScreen);

		/**
		 * Disables the screen. Stops the screen from updating and drawing. Base method that all screens will
		 * implement.
		 */
		virtual void Deactivate();
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();

		virtual void Update(CGameTime*);
		#pragma endregion

		friend class CScreenManager;
};