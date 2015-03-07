#include "render.h"

#include "game.h"
#include "math.h"

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#ifdef _WIN32
		#include <windows.h>
	#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

float camera_angle = 0.0f;

static void drawcoord();

static void rendertile(struct tile *tile)
{
	glPushMatrix();
	glMultMatrixf(tile->transform);
	
	glBegin(GL_QUADS);
	
	glColor3ub(0, 109, 150);
	
	glVertex3f(-1.0, 0.0, 1.0);
	glVertex3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, 0.0, -1.0);
	glVertex3f(-1.0, 0.0, -1.0);
	
	glColor3ub(0, 213, 190);
	
	if(tile->west == -1)
	{
		glVertex3f(-1.0, 0.1, 1.0);
		glVertex3f(-1.0, 0.1, -1.0);
		glVertex3f(-1.0, 0.4, -1.0);
		glVertex3f(-1.0, 0.4, 1.0);
	}
	
	if(tile->north == -1)
	{
		glVertex3f(-1.0, 0.1, -1.0);
		glVertex3f(1.0, 0.1, -1.0);
		glVertex3f(1.0, 0.4, -1.0);
		glVertex3f(-1.0, 0.4, -1.0);
	}
	
	if(tile->east == -1)
	{
		glVertex3f(1.0, 0.1, 1.0);
		glVertex3f(1.0, 0.1, -1.0);
		glVertex3f(1.0, 0.4, -1.0);
		glVertex3f(1.0, 0.4, 1.0);
	}
	
	if(tile->south == -1)
	{
		glVertex3f(-1.0, 0.1, 1.0);
		glVertex3f(1.0, 0.1, 1.0);
		glVertex3f(1.0, 0.4, 1.0);
		glVertex3f(-1.0, 0.4, 1.0);
	}
	
	glEnd();
	
	glPopMatrix();
}

static void renderplayer()
{
	glBegin(GL_QUADS);
	
	glColor3ub(40, 0, 130);
	
	#define V(x,y,z) \
		glVertex3f(x ? -0.3 : 0.3, y ? 0.0 : 0.5, z ? -0.3 : 0.3);
	
	V(0,0,1) V(1,0,1) V(1,1,1) V(0,1,1)
	V(1,0,1) V(1,0,0) V(1,1,0) V(1,1,1)
	V(1,0,0) V(0,0,0) V(0,1,0) V(1,1,0)
	V(0,0,0) V(0,0,1) V(0,1,1) V(0,1,0)
	V(0,0,1) V(1,0,1) V(1,0,0) V(0,0,0)
	V(0,1,1) V(1,1,1) V(1,1,0) V(0,1,0)
	
	#undef V
	
	glEnd();
}

static void rendercheckpoint(GLubyte r, GLubyte g, GLubyte b)
{
	glBegin(GL_TRIANGLE_STRIP);
	
	int i;
	float a;
	for(i=0; i<=16; i++)
	{
		a = (i / 16.0) * (2.0 * M_PI);
		
		glColor4ub(r, g, b, 63);
		glVertex3f(cos(a) * 0.5, 0.0, sin(a) * 0.5);
		glColor4ub(0, 0, 0, 0);
		glVertex3f(cos(a) * 0.5, 5.0, sin(a) * 0.5);
	}
	
	glEnd();
}

static void drawcoord()
{
	glDepthRange(0, 0);
	
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3i(0, 0, 0);
	glVertex3i(1, 0, 0);
	glColor3ub(0, 255, 0);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 1, 0);
	glColor3ub(0, 0, 255);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 1);
	glEnd();
	
	glDepthRange(0, 1);
}

void renderinit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	
	extern int window_w, window_h;
	gluPerspective(60.0, window_w/(float)window_h, 1.0, 100.0);
	
	glMatrixMode(GL_MODELVIEW);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	extern struct tile *tiles;
	extern int tiles_count;
	extern int current_tile;
	extern int start_tile, goal_tile;
	
	extern float player_position[2];
	extern float player_pan;
	
	glLoadIdentity();
	
	glTranslatef(-0.05, -1.0, -3.0);
	glRotatef(camera_angle, 0.0, 1.0, 0.0);
	
	renderplayer();
	
	glRotatef(-player_pan * (180.0 / M_PI), 0.0, 1.0, 0.0);
	glTranslatef(-player_position[0], 0.0, -player_position[1]);
	
	glPushMatrix();
	
	float ls[16];
	inv_orthonormal_m4_m4(ls, tiles[current_tile].transform);
	glMultMatrixf(ls);
	
	int i;
	for(i=0; i<tiles_count; i++)
	{
		rendertile(&tiles[i]);
	}
	
	glEnable(GL_BLEND);
	glDepthMask(0);
	
	//start
	glPushMatrix();
	glMultMatrixf(tiles[start_tile].transform);
	rendercheckpoint(255, 186, 0);
	glPopMatrix();
	
	//goal
	glPushMatrix();
	glMultMatrixf(tiles[goal_tile].transform);
	rendercheckpoint(0, 255, 0);
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDepthMask(1);
	
	glPopMatrix();
}
