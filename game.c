#include "game.h"

#include "math.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

struct tile *tiles;
int tiles_count;

int current_tile;

int start_tile, goal_tile;

float player_position[2];
float player_pan;

int level = 0;
char level_text[1024];

int god;

static int iscommand(const char *command, const char *line)
{
	const char *c = command, *l = line;
	for(; *c && *l && *l != ' ' && *c == *l; c++, l++);
	return *c == '\0' && *l == ' ';
}

static void loadlevel(const char *filename)
{

#if defined __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        printf("cannot change working directory, check permissions\n");
        exit(0);
    }
    CFRelease(resourcesURL);
    
    chdir(path);
#endif
    
	FILE *f = fopen(filename, "r");

    
	if(!f)
	{
		printf("cannot open level file %s\n", filename);
		exit(0);
	}
	
	player_position[0] = player_position[1] = 0.0;
	player_pan = 0.0;
	strcpy(level_text, "[no text]");
	
	int selected_tile = -1;
	int line = 1;
	
	char buf[1024];
	while(fgets(buf, 1024, f))
	{
		#define CMD(s) if(iscommand(s, buf))
		#define ERROR goto parse_error;
		
		CMD("tiles")
		{
			if(sscanf(buf+6, "%d", &tiles_count) != 1) ERROR
			
			tiles = malloc(tiles_count * sizeof(struct tile));
			
			int i;
			for(i=0; i<tiles_count; i++)
			{
				create_identity_m4(tiles[i].transform);
				tiles[i].west = tiles[i].north =
					tiles[i].east = tiles[i].south = -1;
			}
		}
		
		CMD("text")
		{
			strcpy(level_text, buf+5);
		}
		
		CMD("start")
		{
			if(sscanf(buf+6, "%d", &start_tile) != 1) ERROR
			current_tile = start_tile;
		}
		
		CMD("goal")
		{
			if(sscanf(buf+5, "%d", &goal_tile) != 1) ERROR
		}
		
		CMD("pan")
		{
			if(sscanf(buf+4, "%f", &player_pan) != 1) ERROR
			
			player_pan *= 2.0 * M_PI / 360.0;
		}
		
		CMD("tile")
		{
			if(sscanf(buf+5, "%d", &selected_tile) != 1) ERROR
		}
		
		CMD("transform")
		{
			float m[16];
			if(sscanf(buf+10, "%f %f %f %f %f %f %f %f %f %f %f %f "
				"%f %f %f %f", m+0, m+1, m+2, m+3, m+4, m+5,
				m+6, m+7, m+8, m+9, m+10, m+11, m+12, m+13,
				m+14, m+15) != 16) ERROR
			copy_m4_m4(tiles[selected_tile].transform, m);
		}
		
		CMD("transform_by")
		{
			int o;
			if(sscanf(buf+13, "%d", &o) != 1) ERROR
			mul_m4_m4(tiles[selected_tile].transform,
				tiles[o].transform);
		}
		
		CMD("translate")
		{
			float x, y, z;
			if(sscanf(buf+10, "%f %f %f", &x, &y, &z) != 3) ERROR
			translate_m4(tiles[selected_tile].transform, x, y, z);
		}
		
		CMD("rotate_x")
		{
		}
		
		CMD("rotate_y")
		{
		}
		
		CMD("rotate_z")
		{
			float a;
			if(sscanf(buf+9, "%f", &a) != 1) ERROR
			rotate_z_m4(tiles[selected_tile].transform,
				a * 2.0 * M_PI / 360.0);
		}
		
		CMD("west")
		{
			int v;
			if(sscanf(buf+5, "%d", &v) != 1) ERROR
			tiles[selected_tile].west = v;
		}
		
		CMD("north")
		{
			int v;
			if(sscanf(buf+6, "%d", &v) != 1) ERROR
			tiles[selected_tile].north = v;
		}
		
		CMD("east")
		{
			int v;
			if(sscanf(buf+5, "%d", &v) != 1) ERROR
			tiles[selected_tile].east = v;
		}
		
		CMD("south")
		{
			int v;
			if(sscanf(buf+6, "%d", &v) != 1) ERROR
			tiles[selected_tile].south = v;
		}
		
		CMD("west2")
		{
			int v;
			if(sscanf(buf+6, "%d", &v) != 1) ERROR
			tiles[selected_tile].west = v;
			tiles[v].east = selected_tile;
		}
		
		CMD("north2")
		{
			int v;
			if(sscanf(buf+7, "%d", &v) != 1) ERROR
			tiles[selected_tile].north = v;
			tiles[v].south = selected_tile;
		}
		
		CMD("east2")
		{
			int v;
			if(sscanf(buf+6, "%d", &v) != 1) ERROR
			tiles[selected_tile].east = v;
			tiles[v].west = selected_tile;
		}
		
		CMD("south2")
		{
			int v;
			if(sscanf(buf+7, "%d", &v) != 1) ERROR
			tiles[selected_tile].south = v;
			tiles[v].north = selected_tile;
		}
		
		#undef CMD
		#undef ERROR
		
		line++;
	}
	
	fclose(f);
	
	extern SDL_Window *window;
	extern int input_fwd, input_bwd, input_left, input_right;
	
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Level",
		level_text, window);
	input_fwd = input_bwd = input_left = input_right = 0;
	
	return;
	
	parse_error:
	printf("error loading level in line %d\n", line);
	exit(0);
}

void nextlevel()
{
	if(tiles) free(tiles);
	
	level++;
	char filename[16];
	sprintf(filename, "levels/%d", level);
	loadlevel(filename);
}

void initgame()
{
	nextlevel();
	//loadlevel("generator/level");
}

void updategame()
{
	extern int input_fwd, input_bwd, input_left, input_right;
	
	player_position[0] += (input_bwd - input_fwd) * sin(player_pan) * 0.1;
	player_position[1] += (input_bwd - input_fwd) * cos(player_pan) * 0.1;
	
	player_pan += (input_left - input_right) * (M_PI / 90.0);
	
	if(god) goto skip_collisions;
	
	//west
	if(player_position[0] < -1.0)
	{
		if(tiles[current_tile].west == -1) player_position[0] = -1.0;
		else
		{
			current_tile = tiles[current_tile].west;
			player_position[0] += 2.0;
		}
	}
	//north
	if(player_position[1] < -1.0)
	{
		if(tiles[current_tile].north == -1) player_position[1] = -1.0;
		else
		{
			current_tile = tiles[current_tile].north;
			player_position[1] += 2.0;
		}
	}
	//east
	if(player_position[0] > 1.0)
	{
		if(tiles[current_tile].east == -1) player_position[0] = 1.0;
		else
		{
			current_tile = tiles[current_tile].east;
			player_position[0] -= 2.0;
		}
	}
	//south
	if(player_position[1] > 1.0)
	{
		if(tiles[current_tile].south == -1) player_position[1] = 1.0;
		else
		{
			current_tile = tiles[current_tile].south;
			player_position[1] -= 2.0;
		}
	}
	
	skip_collisions:
	
	if(current_tile == goal_tile && len_v2(player_position) <= 0.5)
	{
		nextlevel();
	}
}
