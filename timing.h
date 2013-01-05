#ifndef _TIMING_H_ 
#define _TIMING_H_ 

#include <windows.h> 

// CLASSES 

//! The timing class 
class cTiming 
{ 
public:

	LARGE_INTEGER ticksPerSecond; //frequency
	LARGE_INTEGER tick; // a point in time
	LARGE_INTEGER time;// for converting tick into real time

	LARGE_INTEGER startFrame;
	LARGE_INTEGER stopFrame;
	LARGE_INTEGER currentTick;
	LARGE_INTEGER nFrames;
	LARGE_INTEGER Ticks_To_Wait;
	LARGE_INTEGER tickspassed;
	LARGE_INTEGER ticksleft;
	LARGE_INTEGER prev_frame;

	int done;
	int nFPS;

   cTiming();                        // Constructor 
   void StartTimer();
   void Wait(int MaxFrameRate);
   void FPSloop();
	int GetTicksToWait();
}; 
#endif