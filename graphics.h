#ifndef GRAPHICS_H
#define GRAPHICS_H

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)



class CGraphics
{
private:

	HWND main_window_handle;
	int Screen_Width;
	int Screen_Height;

public:

	LARGE_INTEGER ticksPerSecond; //frequency
	LARGE_INTEGER tick; // a point in time
	LARGE_INTEGER time;// for converting tick into real time

	LARGE_INTEGER startFrame;
	LARGE_INTEGER stopFrame;
//	LARGE_INTEGER currentTick;
	LARGE_INTEGER nFrames;
	LARGE_INTEGER tickstowait;
	LARGE_INTEGER tickspassed;
	LARGE_INTEGER ticksleft;
	LARGE_INTEGER prev_frame;

	int done;
	bool showmenu;
	bool Splash;
	bool up;
	bool down;
	bool right;
	bool left;
	int col;
	int row;
	int x;
	int y;

	int velocity;
//	int velocity_up;
//	int velocity_down;
//	int velocity_left;
//	int velocity_right;
//	int velocity_hori;
//	int velocity_vert;
	bool Init(HWND hwnd);
	bool Paint();
	//bool Paint(HWND hwnd);
	//bool Collecting(int x, int y);
	bool Collecting();
	bool Sick();
	//bool SquirrelSick(HWND hwnd);
	bool FillBag();
	//bool FillBag(HWND hwnd);
	void EmptyBag();
	//void EmptyBag(HWND hwnd);
	bool Status();
	//bool Status(HWND hwnd);
	bool GameFinished();
	//bool GameFinished(HWND hwnd);
	//void EraseScreen(HWND hwnd);
	void EraseScreen();
	//void Text(HWND hwnd);
	void Text();
	void Menu();
	//void Menu(HWND hwnd);
	void Input();
	//void Input(HWND hwnd);
	void SplashScreen();
	//void SplashScreen(HWND hwnd);
	void RandomTest();
	bool CheckForAcorns();

	int test();

	CGraphics();
	CGraphics(int Width, int Height);
	~CGraphics() {};
};
#endif