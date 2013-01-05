#include <iostream>
#include <ctime>
#include <cstdio>
using namespace std;
#include <windows.h>
#include "CVector.h"
#include "graphics.h"
#include "timing.h"
#include "LoadMap.h"
#include "Clevels.h"

/*--------------------------------------------
	GLOBALS
----------------------------------------------*/

struct PLAYER
{
	char Name[64];
	int x;
	int y;
	int Score;
	int Velocity;
	int PrevX;
	int PrevY;
	int LastX;
	int LastY;
	double Acorns;
	HBITMAP hIcon;
	BITMAP Icon;
	
	int Capacity;
	bool BadAcorn;
	
} Player, Player2;

// handle till minnes device context
HDC Tiles_DC = NULL;
struct TILES
{
	HBITMAP hBadAcorn;
	BITMAP BadAcorn;
	HBITMAP hAcorn;
	BITMAP Acorn;
	HBITMAP hWall;
	HBITMAP hExit;
	BITMAP Wall;
	BITMAP Exit;

	HBITMAP hIntro;
	BITMAP Intro;
} Tile;

RECT rect;
RECT clear;
RECT object;
RECT player;
// For main handle to device context
HDC gDC = 0;
// for back (double) buffering

HDC		BackBuffer = 0;	
HBITMAP BackBuffer_bmp = 0;
HBITMAP Old_bmp = 0;

static int Bag[3][3];

cTiming FPScounter;

// Initiating everything for the start of the game
bool CGraphics::Init(HWND hwnd)
{
	main_window_handle = hwnd;

	//CLevels* leveltest = 0;

//-----------------------------------------------------------------
// Player 1 related stuff
//-----------------------------------------------------------------
	Player.BadAcorn = false;
	Player.Velocity = 1;
	Player.Acorns = 0;
	Player.Capacity = 9;
	Player.x = 576;
	Player.y = 448;
	Player.Score = 0;

	// Den används av både player 1 och 2
	Player.hIcon = (HBITMAP)LoadImage(NULL,"Badger.bmp",
		IMAGE_BITMAP, 0, 0,	LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if( Player.hIcon == 0 )
	{
		MessageBox(hwnd,TEXT("Error, no player.bmp in directory\n"), TEXT("Error"),MB_ICONERROR | MB_OK);
		return false;
	}	
	
	GetObject(Player.hIcon,sizeof(BITMAP), &Player.Icon);
//------------------------------------------------------------------

//-----------------------------------------------------------------
// Player 2 related stuff
//-----------------------------------------------------------------
	Player2.BadAcorn = false;
	Player2.Velocity = 1;
	Player2.Acorns = 0;
	Player2.Capacity = 9;
	Player2.x = 576;
	Player2.y = 0;
//------------------------------------------------------------------

//------------------------------------------------------------------
// Time related initiation
//------------------------------------------------------------------
	QueryPerformanceFrequency(&ticksPerSecond);
	QueryPerformanceFrequency(&FPScounter.ticksPerSecond);
//------------------------------------------------------------------

//------------------------------------------------------------------
// Splash screen stuff
//------------------------------------------------------------------
	Splash = true;

	Tile.hIntro = (HBITMAP)LoadImage(NULL, "Splash.bmp",
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if( Tile.hIntro == 0 )
	{
		MessageBox(hwnd,TEXT("Error, no Acorn.bmp in directory\n"), TEXT("Error"),MB_ICONERROR | MB_OK);
		return false;
	}
//------------------------------------------------------------------
	
	col = 0;
	row = 0;
	
	showmenu = false;

	gDC = GetDC(hwnd);

	// För BackBuffer (Dubbel) buffring
	BackBuffer = CreateCompatibleDC(gDC);
	BackBuffer_bmp = CreateCompatibleBitmap(gDC, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	Old_bmp = (HBITMAP)SelectObject(BackBuffer, BackBuffer_bmp);

	//-------------------------------------------------------------------
	// Ladddar in bmp bilder för alla objekt i spelet


	Tile.hBadAcorn = (HBITMAP)LoadImage(NULL, "BadAcorns.bmp",
                                    IMAGE_BITMAP, 0, 0,
                                    LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if( Tile.hBadAcorn == 0 )
	{
		MessageBox(hwnd,TEXT("Error, no BadAcorns.bmp in directory\n"), TEXT("Error"),MB_ICONERROR | MB_OK);
		return false;
	}

	Tile.hAcorn = (HBITMAP)LoadImage(NULL, "Acorns.bmp",
                                    IMAGE_BITMAP, 0, 0,
                                    LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if( Tile.hAcorn == 0 )
	{
		MessageBox(hwnd,TEXT("Error, no Acorn.bmp in directory\n"), TEXT("Error"),MB_ICONERROR | MB_OK);
		return false;
	}

	// För BackBuffer (Dubbel) buffring
	BackBuffer = CreateCompatibleDC(gDC);
	BackBuffer_bmp = CreateCompatibleBitmap(gDC, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	Old_bmp = (HBITMAP)SelectObject(BackBuffer, BackBuffer_bmp);
	
	Tiles_DC = CreateCompatibleDC(NULL);


	GetObject(Tile.hIntro, sizeof(BITMAP), &Tile.Intro);
	GetObject(Tile.hBadAcorn, sizeof(BITMAP), &Tile.BadAcorn);
	GetObject(Tile.hAcorn, sizeof(BITMAP), &Tile.Acorn);

	RandomTest();
	return true;
}

void CGraphics::EraseScreen()
{
	ValidateRect(main_window_handle,&clear);
	GetClientRect(main_window_handle,&clear);
	FillRect(BackBuffer, &clear, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

void CGraphics::SplashScreen()
{
	if(Splash)
	{
		SelectObject(Tiles_DC,Tile.hIntro);
		BitBlt(BackBuffer, 0, 0, 
		Tile.Intro.bmWidth, Tile.Intro.bmHeight, Tiles_DC, 0, 0, SRCCOPY);

		BitBlt(gDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
			GetSystemMetrics(SM_CYSCREEN), BackBuffer, 0, 0, SRCCOPY);
	}
	if(KEY_DOWN(VK_SPACE))
		Splash = false;
}

void CGraphics::RandomTest()
{
	for(col = 0; col < 15; col++)
	{
		for(row = 0; row < 15; row++)
		{
			levelFigure[row][col] = rand() % 4;
		}
	}
}

bool CGraphics::CheckForAcorns()
{
	for(col = 0; col < 15; col++)
	{
		for(row = 0; row < 15; row++)
		{
			if(levelFigure[row][col] == 1)
			{
				return true;
			}
		}
	}
	return false;
}

void CGraphics::Menu()
{
	if(showmenu)
	{
		SetTextColor(BackBuffer,RGB(255,0,0));
		SetBkColor(BackBuffer,RGB(255,255,255));

		GetClientRect(main_window_handle,&rect);

		rect.top = y/3;
		DrawText(BackBuffer,"Create Character\nLoad Character\nCharacter List\nHall of records\nHelp Lesson\nTutorial\nOptions\nExit",-1,&rect, DT_CENTER);

		SetTextColor(BackBuffer,RGB(0,255,0));

		
		if(KEY_DOWN('1'))
		{
			SetWindowPos(main_window_handle, 0,0,0,320,240,0);
			DrawText(BackBuffer,"Create Character",-1, &rect, DT_CENTER);
		}

		if(KEY_DOWN('2'))
		{
			SetWindowPos(main_window_handle, 0,0,0,640,480,0);
			DrawText(BackBuffer,"\nLoad Character",-1, &rect, DT_CENTER);
		}

		if(KEY_DOWN('3'))
		{
			DrawText(BackBuffer,"\n\nCharacter List",-1, &rect, DT_CENTER);
		}
		
		if(KEY_DOWN('4'))
		{
			DrawText(BackBuffer,"\n\n\nHall of records",-1, &rect, DT_CENTER);
		}
		
		if(KEY_DOWN('5'))
		{
			DrawText(BackBuffer,"\n\n\n\nHelp Lesson",-1, &rect, DT_CENTER);
		}
		
		if(KEY_DOWN('6'))
		{
			DrawText(BackBuffer,"\n\n\n\n\nTutorial",-1, &rect, DT_CENTER);
		}
		
		if(KEY_DOWN('7'))
		{
			DrawText(BackBuffer,"\n\n\n\n\n\nOptions",-1, &rect, DT_CENTER);
		}

		if(KEY_DOWN('8'))
		{
			showmenu = false;
		}

		BitBlt(gDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
			GetSystemMetrics(SM_CYSCREEN), BackBuffer, 0, 0, SRCCOPY);
	}
}

void CGraphics::Input()
{
	// Movement keys for player 1
	if(KEY_DOWN(VK_UP) && Player.y > 0)
	{
		Player.y -= Player.Velocity;
	}
	if(KEY_UP(VK_UP))
		up = false;

	if(KEY_DOWN(VK_DOWN) && Player.y < 448)
	{
		Player.y += Player.Velocity;
	}
	if(KEY_UP(VK_DOWN))
		down = false;

	if(KEY_DOWN(VK_RIGHT) && Player.x < 576)
	{
		Player.x += Player.Velocity;
	}
	if(KEY_UP(VK_RIGHT))
		right = false;

	if(KEY_DOWN(VK_LEFT) && Player.x > 0)
	{
		Player.x -= Player.Velocity;
	}
	if(KEY_UP(VK_LEFT))
		left = false;
	///////////////////////////////////////////

	// Movement keys for player 2
	if(KEY_DOWN('W') && Player2.y > 0)
	{
		Player2.y -= Player2.Velocity;
	}
	if(KEY_UP('W'))
		up = false;

	if(KEY_DOWN('S') && Player2.y < 448)
	{
		Player2.y += Player2.Velocity;
	}
	if(KEY_UP('S'))
		down = false;

	if(KEY_DOWN('D') && Player2.x < 576)
	{
		Player2.x += Player2.Velocity;
	}
	if(KEY_UP('D'))
		right = false;

	if(KEY_DOWN('A') && Player2.x > 0)
	{
		Player2.x -= Player2.Velocity;
	}
	if(KEY_UP('A'))
		left = false;
	///////////////////////////////////////////

	// Pressing Escape to quit game
	if(KEY_DOWN(VK_ESCAPE))
		PostQuitMessage(0);

	// F3 to show game menu
	if(KEY_DOWN(VK_F3))
		showmenu = true;
}
bool CGraphics::Paint()
{
	FPScounter.StartTimer();

	// Paints bag of Acorns
	for(col = 0; col < 3; col++)
	{
		for(row = 0; row < 3; row++)
		{
			if(Bag[col][row] == 1)
			{
				SelectObject(Tiles_DC,Tile.hBadAcorn);
				BitBlt(BackBuffer, 547 + col*Tile.BadAcorn.bmWidth, 0 + row*Tile.BadAcorn.bmHeight, 
				Tile.BadAcorn.bmWidth, Tile.BadAcorn.bmHeight, Tiles_DC, 0, 0, SRCAND);
			}
		}
	}

	// Places the different Acorns on the playing field
	for(col = 0; col < 15; col++)
	{
		for(row = 0; row < 15; row++)
		{
			if(levelFigure[col][row] == 3)
			{
				SelectObject(Tiles_DC,Tile.hBadAcorn);

				BitBlt(BackBuffer, col*Tile.BadAcorn.bmWidth, row*Tile.BadAcorn.bmWidth, 
					Tile.BadAcorn.bmWidth, Tile.BadAcorn.bmHeight, Tiles_DC, 0, 0, SRCAND);
			}
			if(levelFigure[col][row] == 1)
			{
				SelectObject(Tiles_DC,Tile.hAcorn);

				BitBlt(BackBuffer, col*Tile.Acorn.bmWidth, row*Tile.Acorn.bmWidth, 
					Tile.Acorn.bmWidth, Tile.Acorn.bmHeight, Tiles_DC, 0, 0, SRCAND);
			}
		}
	}
	///////////////////////////////////////////////

	// If the player is at the right edge of the screen empty the bag 
	if(Player.x > 600)
	{
		EmptyBag();
		Player.Capacity = 9;
	}
	
	// Selects the Player bitmap and paints the player in the backbuffer 			
	SelectObject(Tiles_DC,Player.hIcon);

	BitBlt(BackBuffer, Player.x, Player.y, Player.Icon.bmWidth, Player.Icon.bmHeight, Tiles_DC, 0, 0, SRCAND);	

	BitBlt(BackBuffer, Player2.x, Player2.y, Player.Icon.bmWidth, Player.Icon.bmHeight, Tiles_DC, 0, 0, SRCAND);	

	////////////////////////////////////////////////////////

	//Page flipping
	BitBlt(gDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), BackBuffer, 0, 0, SRCCOPY);

	//fps counter
	Status();

	return true;
}

bool CGraphics::Collecting()
{
	// Player 1 stuff
	int xid = Player.x/32;
	int yid = Player.y/32;
	if(levelFigure[xid][yid] == 1)
	{
		levelFigure[xid][yid] = 0;
		Player.Score += 100;
		Player.Acorns += 1;
		Player.BadAcorn = false;
		Sick();
//		return true;
	}
	else if( levelFigure[xid][yid] == 3)
	{
		levelFigure[xid][yid] = 0;
		Player.Score -= 200;
		Player.Acorns -= 1;
		Player.BadAcorn = true;
		Sick();
//		return true;
	}

//	return false;
	//-----------------------------
	
	// Player 2 stuff
	int xid2 = Player2.x/32;
	int yid2 = Player2.y/32;
	if(levelFigure[xid2][yid2] == 1)
	{
		levelFigure[xid2][yid2] = 0;
		Player2.Score += 100;
		Player2.Acorns += 1;
		Player2.BadAcorn = false;
		Sick();
//		return true;
	}
	else if( levelFigure[xid2][yid2] == 3)
	{
		levelFigure[xid2][yid2] = 0;
		Player2.Score -= 200;
		Player2.Acorns -= 1;
		Player2.BadAcorn = true;
		Sick();
//		return true;
	}

	return true;
	//-----------------------------
}

bool CGraphics::Status()
{
	FPScounter.Wait(120);
	FPScounter.FPSloop();
	return true;
}

bool CGraphics::GameFinished()
{
		SetTextColor(BackBuffer,RGB(255,0,0));
		SetBkColor(BackBuffer,RGB(255,255,255));

		GetClientRect(main_window_handle,&rect);

		rect.top = 240;
		if(Player.Acorns > Player2.Acorns)
			DrawText(BackBuffer,"Player won!",-1,&rect, DT_CENTER);
		else
			DrawText(BackBuffer,"Player2 won!",-1,&rect, DT_CENTER);

		BitBlt(gDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
			GetSystemMetrics(SM_CYSCREEN), BackBuffer, 0, 0, SRCCOPY);

		while(!KEY_DOWN(VK_SPACE))
		{
		}
		PostQuitMessage(0);

	return true;
}

bool CGraphics::Sick()
{
	// Check if player 1 is sick
	if(Player.BadAcorn)
	{
		Player.Velocity = 1;
	}
	else
		Player.Velocity = 2;

	if(Player2.BadAcorn)
	{
		Player2.Velocity = 1;
	}
	else
		Player2.Velocity = 2;

	return true;
}

bool CGraphics::FillBag()
{
	if(Player.Capacity == 8)
		Bag[0][0] = 1;
	if(Player.Capacity == 7)
		Bag[0][1] = 1;
	if(Player.Capacity == 6)
		Bag[0][2] = 1;
	if(Player.Capacity == 5)
		Bag[1][0] = 1;
	if(Player.Capacity == 4)
		Bag[1][1] = 1;
	if(Player.Capacity == 3)
		Bag[1][2] = 1;
	if(Player.Capacity == 2)
		Bag[2][0] = 1;
	if(Player.Capacity == 1)
		Bag[2][1] = 1;
	if(Player.Capacity == 0)
		Bag[2][2] = 1;

	return true;
}

//Resetting the bag to be empty
void CGraphics::EmptyBag()
{
	for(col = 0; col < 3; col++)
	{
		for(row = 0; row < 3; row++)
		{
			if(Bag[col][row] == 1)
			{
				Bag[col][row] = 0;
			}
		}
	}
}

void CGraphics::Text()
{
	char buffer[100];

	RECT PlayerStats;
	RECT Player2Stats;

	PlayerStats.bottom = 505;
	PlayerStats.top = 485;
	PlayerStats.left =	0;
	PlayerStats.right = 320;

	Player2Stats.bottom = 505;
	Player2Stats.top = 485;
	Player2Stats.left =	320;
	Player2Stats.right = 640;

	SetTextColor(BackBuffer,RGB(255,0,0));
	SetBkColor(BackBuffer,RGB(255,255,255));

	sprintf(buffer,"Player 1 Score: %d Acorns: %0.f",Player.Score, Player.Acorns);
	DrawText(BackBuffer,buffer,-1,&PlayerStats, 0);

	sprintf(buffer,"Player 2 Score: %d Acorns: %0.f",Player2.Score, Player2.Acorns);
	DrawText(BackBuffer,buffer,-1,&Player2Stats, 0);

	sprintf(buffer,"Player 2 Score: %d",x);
	DrawText(BackBuffer,buffer,-1,&Player2Stats, 0);
/*	sprintf(buffer," PlayerX: %d PlayerY: %d", (Player2.x+32)/32, (Player2.y+32)/32);
	DrawText(BackBuffer,buffer,-1, &TextWindow, 0);
	sprintf(buffer," PlayerX: %d PlayerY: %d", (Player2.x), (Player2.y));
	DrawText(BackBuffer,buffer,-1, &TextWindow, 0);*/
}

int CGraphics::test()
{
	return 320;
}

CGraphics::CGraphics()
{
}

CGraphics::CGraphics(int Width, int Height)
{
}


