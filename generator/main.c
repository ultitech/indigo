#include "maze.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define WIDTH 8
#define HEIGHT 5

static int mod(int x, int d)
{
	if(x < 0) return d + x;
	return x % d;
}

int main(int argc, char *argv[])
{
	int seed = time(NULL);
	srand(seed);
	printf("seed: %d\n", seed);
	
	Maze *maze = maze_generate(WIDTH, HEIGHT, 1, 0);
	maze_print(maze);
	
	FILE *out = fopen("level", "w");
	
	fprintf(out, "tiles %d\n\n", WIDTH*HEIGHT);
	
	int x, y;
	for(y=0; y<HEIGHT; y++) for(x=0; x<WIDTH; x++)
	{
		fprintf(out, "tile %d\n", x+y*WIDTH);
		fprintf(out, "rotate_z %f\n", (x/(float)WIDTH) * 360.0);
		fprintf(out, "translate %f %f %f\n", 2.55, 0.0, y*-2.0);
		fprintf(out, "rotate_z %f\n", 90.0);
		
		Cell *cell = maze_get_cell(maze, x, HEIGHT-1-y);
		if(cell->right) fprintf(out, "east %d\n", mod(x+1, WIDTH)+y*WIDTH);
		if(cell->left) fprintf(out, "west %d\n", mod(x-1, WIDTH)+y*WIDTH);
		if(cell->up) fprintf(out, "north %d\n", x+mod(y+1, HEIGHT)*WIDTH);
		if(cell->down) fprintf(out, "south %d\n", x+mod(y-1, HEIGHT)*WIDTH);
		
		//fputc('\n', out);
	}
	
	fprintf(out, "start 0\ngoal %d", WIDTH*HEIGHT-WIDTH+1);
	
	fclose(out);
}
