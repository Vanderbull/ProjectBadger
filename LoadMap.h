//-------------------------------------------------------------------------
// Namn: Rickard Skeppström
// Personnr: 790930-xxxx
// Klass: SP6
// E-mail: rickard_skeppstrom@msn.com alt rickard.skeppstrom@powerhouse.net
// Lab: Labyrint
//-------------------------------------------------------------------------

#include <fstream>

using namespace std;

#ifndef INC_LOADMAP_H
#define INC_LOADMAP_H

int		levelFigure[20][15];

BOOL LoadMap(HWND hwnd, char *MapFile)
{
	int		col;
	int		row;
	char	c;

	ifstream Map_file ( MapFile, ios_base::in);

	if ( !Map_file.is_open() ) 
	{
		return FALSE;
	}

	for(col = 0; col < 15; col++)
	{
		for(row = 0; row < 20; row++)
		{
			//hämtar in tecken
			c = Map_file.get();
			//konverterar tecknet till integer
			levelFigure[row][col] = atoi(&c);
		}
	}
	
	Map_file.close();

	return TRUE;
}

#endif
