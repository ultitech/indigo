#include "render.h"
#include "game.h"

#include <SDL.h>

int window_w, window_h;
SDL_Window *window;
SDL_GLContext gl_context;

int input_fwd, input_bwd, input_left, input_right;

#pragma GCC diagnostic ignored "-Wswitch"

static int handleevents()
{
	SDL_Event ev;
	
	while(SDL_PollEvent(&ev))
	{
		if(ev.type == SDL_QUIT) return 0;
		
		if(ev.type == SDL_KEYDOWN)
		{
			extern struct tile *tiles;
			extern int current_tile;
			
			extern int god;
			
			extern void nextlevel();
			
			switch(ev.key.keysym.scancode)
			{
				/*
				case SDL_SCANCODE_A:
				if(tiles[current_tile].west != -1)
					current_tile = tiles[current_tile].west;
				break;
				
				case SDL_SCANCODE_W:
				if(tiles[current_tile].north != -1)
					current_tile = tiles[current_tile].north;
				break;
				
				case SDL_SCANCODE_D:
				if(tiles[current_tile].east != -1)
					current_tile = tiles[current_tile].east;
				break;
				
				case SDL_SCANCODE_S:
				if(tiles[current_tile].south != -1)
					current_tile = tiles[current_tile].south;
				break;
				*/
				
				case SDL_SCANCODE_G:
				god ^= 1;
				break;
				
				case SDL_SCANCODE_N:
				nextlevel();
				break;
				
				case SDL_SCANCODE_UP:
				input_fwd = 1;
				break;
				
				case SDL_SCANCODE_DOWN:
				input_bwd = 1;
				break;
				
				case SDL_SCANCODE_LEFT:
				input_left = 1;
				break;
				
				case SDL_SCANCODE_RIGHT:
				input_right = 1;
				break;
			}
		}
		
		if(ev.type == SDL_KEYUP)
		{
			switch(ev.key.keysym.scancode)
			{
				case SDL_SCANCODE_UP:
				input_fwd = 0;
				break;
				
				case SDL_SCANCODE_DOWN:
				input_bwd = 0;
				break;
				
				case SDL_SCANCODE_LEFT:
				input_left = 0;
				break;
				
				case SDL_SCANCODE_RIGHT:
				input_right = 0;
				break;
			}
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	
	extern float camera_angle;
	camera_angle += state[SDL_SCANCODE_Q] - state[SDL_SCANCODE_E];
	
	return 1;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	
	window_w = 1280;
	window_h = 720;
	window = SDL_CreateWindow("indigo", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, window_w, window_h, SDL_WINDOW_OPENGL);
	gl_context = SDL_GL_CreateContext(window);
	
	renderinit();
	
	initgame();
	
	for(;;)
	{
		if(handleevents() == 0) break;
		
		updategame();
		
		render();
		
		SDL_GL_SwapWindow(window);
		
		SDL_Delay(16);
	}
	
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
	return 0;
}
