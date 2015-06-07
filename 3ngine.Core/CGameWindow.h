#include "3ngine.Core.h"

class CORE_API CGameWindow
{
	private:
		HWND m_hWnd;
		HDC m_hDeviceContext;
		HINSTANCE m_hInst;
		WNDCLASS m_sWndClass;
		DWORD m_dwStyles;
		DWORD m_dwExStyles;

		char* m_sTitle;
		RECT m_rcDisplayRect;
		RECT m_rcClientRect;

		CGraphicsDevice* m_pDevice;

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

		void InitializeClass(WNDCLASS*);
		HRESULT InternalCreateWindow();
		void InternalDestroyWindow();
	
	public:
		CGameWindow();
		~CGameWindow();

		void Show();
		void Close();

		#pragma region Properties
		HWND Get_Handle();
		HINSTANCE Get_Instance();

		RECT Get_DisplayRectangle();
		void Set_DisplayRectangle(RECT);

		RECT Get_ClientRectangle();
		void Set_ClientRectangle(RECT);
		#pragma endregion

	protected:
		HRESULT CreateHandle();
		void DestroyHandle();

		friend class CGame;
		friend class CGraphicsDevice;
};