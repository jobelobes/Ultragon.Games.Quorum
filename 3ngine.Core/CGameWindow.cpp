#include "3ngine.Core.h"

#pragma region Static Methods
LRESULT CGameWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CGameWindow* objPtn = (CGameWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch( uMsg )
	{
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}
		case WM_CLOSE:
		{
			::PostQuitMessage(0);
			return 0;
		}
		case WM_EXITSIZEMOVE:
		{
			WINDOWINFO info;
			::GetWindowInfo(hWnd, &info);
			objPtn->m_rcDisplayRect = info.rcWindow;
			objPtn->m_rcClientRect = info.rcClient;
			objPtn->m_pDevice->Resize(info.rcClient.right - info.rcClient.left, info.rcClient.bottom - info.rcClient.top);
		}
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#pragma endregion

#pragma region Constructors and Finalizers
CGameWindow::CGameWindow()
{
	this->m_hInst = 0;
	this->m_hWnd = 0;
	this->m_sTitle = "GameWindow1";

	this->m_rcClientRect.left = 100;
	this->m_rcClientRect.top = 100;
	this->m_rcClientRect.right = 740;
	this->m_rcClientRect.bottom = 580;

	this->m_rcDisplayRect.left = this->m_rcClientRect.left;
	this->m_rcDisplayRect.top = this->m_rcClientRect.top;
	this->m_rcDisplayRect.right = this->m_rcClientRect.right;
	this->m_rcDisplayRect.bottom = this->m_rcClientRect.bottom;
}
CGameWindow::~CGameWindow()
{
	this->InternalDestroyWindow();
	this->DestroyHandle();
}
#pragma endregion

#pragma region Methods
void CGameWindow::InitializeClass(WNDCLASS* wc)
{
	wc->style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc->lpfnWndProc		= (WNDPROC)(CGameWindow::WndProc);
	wc->cbClsExtra		= 0;
	wc->cbWndExtra		= 0;
	wc->hInstance		= this->m_hInst;
	wc->hIcon			= ::LoadIcon(NULL, IDI_WINLOGO);
	wc->hCursor			= ::LoadCursor(NULL, IDC_ARROW);
	wc->hbrBackground	= NULL;
	wc->lpszMenuName	= NULL;
	wc->lpszClassName	= (LPCTSTR)"Direct 3D GameWindow";
}

HRESULT CGameWindow::CreateHandle()
{
	this->m_hInst = ::GetModuleHandle(NULL);

	// create and initialize the window class
	this->InitializeClass(&this->m_sWndClass);
	if (!::RegisterClass(&this->m_sWndClass))
		return S_FALSE;

	// change the styles for fullscreen or windowed mode
	this->m_dwExStyles = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
	this->m_dwStyles = WS_OVERLAPPEDWINDOW;	

	::AdjustWindowRectEx(&this->m_rcDisplayRect, this->m_dwStyles, FALSE, this->m_dwExStyles);

	return S_OK;
}

void CGameWindow::DestroyHandle()
{
	if (!::UnregisterClass(this->m_sWndClass.lpszClassName, this->m_hInst))
		this->m_hInst = NULL;
}

HRESULT CGameWindow::InternalCreateWindow()
{
	// create the actual window from the style def and class
	this->m_hWnd = ::CreateWindowEx(
		this->m_dwExStyles,
		this->m_sWndClass.lpszClassName,
		this->m_sTitle,
		this->m_dwStyles | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		this->m_rcDisplayRect.left,
		this->m_rcDisplayRect.top,
		this->m_rcDisplayRect.right - this->m_rcDisplayRect.left,
		this->m_rcDisplayRect.bottom - this->m_rcDisplayRect.top,
		NULL,
		NULL,
		this->m_hInst,
		NULL);
	if (!this->m_hWnd)
		return S_FALSE;

	// set user data
	::SetWindowLongPtr(this->m_hWnd, GWLP_USERDATA, (long)this);

	// get the device context
	this->m_hDeviceContext = ::GetDC(this->m_hWnd);
	if (!this->m_hDeviceContext)
		return S_FALSE;

	return S_OK;
}


void CGameWindow::InternalDestroyWindow()
{
	// release the contexts
	if (this->m_hDeviceContext && !ReleaseDC(this->m_hWnd, this->m_hDeviceContext))	
		this->m_hDeviceContext = NULL;
	if (this->m_hWnd && !::DestroyWindow(this->m_hWnd))
		this->m_hWnd =NULL;
}

void CGameWindow::Show()
{
	if(!this->m_hInst)
		this->CreateHandle();

	if(!this->m_hWnd)
		this->InternalCreateWindow();

	::ShowWindow(this->m_hWnd, SW_SHOW);
	::SetForegroundWindow(this->m_hWnd);
	::SetFocus(this->m_hWnd);	
}

void CGameWindow::Close()
{
	if(!this->m_hWnd)
		this->InternalDestroyWindow();

	if(this->m_hInst)
		this->DestroyHandle();
}
#pragma endregion

#pragma region Properties
HWND CGameWindow::Get_Handle()
{
	return this->m_hWnd;
}

HINSTANCE CGameWindow::Get_Instance()
{
	return this->m_hInst;
}

RECT CGameWindow::Get_DisplayRectangle()
{
	WINDOWINFO info;
	::GetWindowInfo(this->m_hWnd, &info);
	return info.rcWindow;
}

RECT CGameWindow::Get_ClientRectangle()
{
	WINDOWINFO info;
	::GetWindowInfo(this->m_hWnd, &info);
	return info.rcClient;
}

void CGameWindow::Set_ClientRectangle(RECT value)
{
	this->m_rcClientRect = value;
	this->m_rcDisplayRect.left = this->m_rcClientRect.left;
	this->m_rcDisplayRect.top = this->m_rcClientRect.top;
	this->m_rcDisplayRect.right = this->m_rcClientRect.right;
	this->m_rcDisplayRect.bottom = this->m_rcClientRect.bottom;

	::AdjustWindowRectEx(&this->m_rcDisplayRect, this->m_dwStyles, FALSE, this->m_dwExStyles);
}
#pragma endregion
