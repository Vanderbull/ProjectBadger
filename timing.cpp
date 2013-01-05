// INCLUDES 

#include <windows.h> 
//#include <mmsystem.h> 
#include "Timing.h" 
#include <time.h> 

// GLOBALS 

// CONSTRUCTORS 

// Constructor 
cTiming::cTiming() 
{
	LARGE_INTEGER ticksPerSecond = {0};
	LARGE_INTEGER tick = {0}; // a point in time
	LARGE_INTEGER time = {0};// for converting tick into real time

	LARGE_INTEGER startFrame = {0};
	LARGE_INTEGER stopFrame = {0};
	LARGE_INTEGER currentTick = {0};
	LARGE_INTEGER nFrames = {0};
	LARGE_INTEGER Ticks_To_Wait = {0};
	LARGE_INTEGER tickspassed = {0};
	LARGE_INTEGER ticksleft = {0};
	LARGE_INTEGER prev_frame = {0};

	int done = 0;

   // Set current and previous time to the clock time at initialisation. 
   // Otherwise mDelta = mCurrentTime - mPrevious in CalculateFrameRate() will 
   // give an absurd number during its first call and screw up all the physics 
   QueryPerformanceFrequency(&ticksPerSecond); 
}// cTiming

void cTiming::StartTimer()
{
	QueryPerformanceCounter(&startFrame);
}

void cTiming::Wait(int MaxFrameRate)
{
	Ticks_To_Wait.QuadPart = ticksPerSecond.QuadPart/MaxFrameRate;
}

void cTiming::FPSloop()
{
	done = 0;
	do
	{
		QueryPerformanceCounter(&time);
		tickspassed.QuadPart = time.QuadPart - prev_frame.QuadPart;
		ticksleft.QuadPart = Ticks_To_Wait.QuadPart - tickspassed.QuadPart;

		if(time.QuadPart < prev_frame.QuadPart)
			done = 1;
		if(tickspassed.QuadPart >= Ticks_To_Wait.QuadPart)
			done = 1;
	}
	while(!done);

	prev_frame = time;
}

int cTiming::GetTicksToWait()
{
	return (int)((__int64)Ticks_To_Wait.QuadPart);
}