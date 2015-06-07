#include "Quorum.h"

//int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
//{
//	CGame1* game = new CGame1();
//	game->Run();
//}

int _tmain(int argc, _TCHAR* argv[])
{
	CGame1* game = new CGame1();
	game->Run();
	return GetLastError();
}

