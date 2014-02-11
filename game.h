#ifndef H_GAME
#define H_GAME

struct tile
{
	float transform[16];
	int west, north, east, south;
};

void initgame();
void updategame();

#endif //H_GAME
