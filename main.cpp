//-------------------------------------------------------------------------
// Namn: Rickard Skeppström
// Personnr: 790930-xxxx
// Klass: SP6
// E-mail: rickard_skeppstrom@msn.com alt rickard.skeppstrom@powerhouse.net
// Lab: Labyrint
//-------------------------------------------------------------------------

#include <windows.h>
#include "resource.h"
#include <ctime>
#include "graphics.h"


/**********************************************************************
* Globaler
/*********************************************************************/
CGraphics* test = 0;

int width = 640;

HDC hDC = 0;

int game_state = 0;
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

/***************************************************************************************
	Main window entry point
****************************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR szCmdLine, int iCmdShow) 
{
	TCHAR szAppName[] = "Labbet";
	HWND hwnd;
	MSG			msg = {0};
	WNDCLASS	wndclass ;

	wndclass.style			= CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc	= WndProc ;
	wndclass.cbClsExtra		= 0 ;
	wndclass.cbWndExtra		= 0 ;
	wndclass.hInstance		= hInstance ;
	wndclass.hIcon			= LoadIcon (hInstance, IDI_APPLICATION) ;
	wndclass.hCursor		= LoadCursor (hInstance, IDC_ARROW) ;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (BLACK_BRUSH) ;
	wndclass.lpszMenuName	= NULL ;
	wndclass.lpszClassName	= szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires a advanced windows"),
					szAppName, MB_ICONERROR) ;
		return 0;
	}
	
	test = new CGraphics(640,480);
	

	// Huvudfönstret
	hwnd = CreateWindow (szAppName,		// Window class name
						 TEXT ("Labbet"),	//window caption
						 WS_POPUP | WS_BORDER,
						 0,
						 0,
						 GetSystemMetrics(SM_CXSCREEN),
						 GetSystemMetrics(SM_CYSCREEN),
						 NULL,
						 NULL,
						 hInstance,
						 NULL) ;

	srand ( time(0) );

	ShowWindow(hwnd, iCmdShow) ;
	UpdateWindow(hwnd);

	test->Init(hwnd);
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage (&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			test->EraseScreen();
			test->Text();

			if( (!test->Splash) && (!test->showmenu))
				test->Input();

			test->Sick();
			//test->Collecting(test->x/32,test->y/32);
			test->Collecting();
			if(test->Splash)
				test->SplashScreen();

			if(test->showmenu)
				test->Menu();
			if(!test->Splash && !test->showmenu)
				test->Paint();
		
			if(!test->CheckForAcorns())
			{
				test->GameFinished();
			}
		}
	}
	return(msg.wParam) ;
}

/*******************************************************************************
* LRESULT CALLBACK WndProc() sköter grovjobbet
/*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
//	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_SIZE:
		test->x = LOWORD(lParam);
		test->y = HIWORD(lParam);
		return 0;

	case WM_QUIT:
		return 0;

	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}