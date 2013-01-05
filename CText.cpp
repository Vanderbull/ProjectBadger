// INCLUDES 

#include <windows.h> 
//#include <mmsystem.h> 
#include "CText.h" 

// GLOBALS 
HDC text;
// CONSTRUCTORS 

// Constructor 
cText::cText() 
{
}// cText

void cText::Sick()
{
	TextOut(text, 450,300,"SICK!",strlen("SICK!"));
}
