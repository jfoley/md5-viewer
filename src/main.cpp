#ifdef _WIN32
	#include <windows.h>
	#include "GL\glee.h"
	#include "GL\glu.h"
	
	const char* directory_seperator = "\\";
	const char* model = "hellknight";
#endif

#ifdef __APPLE__
	#include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
	const char* directory_seperator = "/";
	const char* model = "hellknight";
#else
	#define __LINUX__
	#include <GL/glew.h>
#endif

#include <math.h>
#include <fstream>
#include <string>
#include <exception>
#include <SDL.h>

#include "MD5.h"
#include "Camera.h"
#include "Engine.h"
#include "Timer.h"
#include "Shader.h"

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

Engine engine;
const int FRAMES_PER_SECOND = 60;
bool left_down = false;
bool right_down = false;
bool light_move = true;

float x_rot, y_rot;


float zh=0.0;        //  Light azimuth
float Ylight=120;   //  Light elevation

int frame = 0;
int prog, vert, frag = 0;

MD5 md5;
Camera cam;
GLUquadric* quad;

int init(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	if( SDL_SetVideoMode( 640, 480, 32, SDL_OPENGL ) < 0 )
    {
		exit(1);
        return false;
    }
	SDL_WM_SetCaption( "MD5 Loader", NULL );

	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 0.01,  1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	md5.Load("models/hellknight/hellknight.md5mesh");
	md5.LoadAnimation("models/hellknight/idle2.md5anim");

	prog = CreateShaderProg("src/shaders/bump.vert", "src/shaders/bump.frag");
	quad = gluNewQuadric();
	return true;
}

void draw_quad() {
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glNormal3f( 0, 0,+1);
	glTexCoord2f(0,0); glVertex3f(-100,-100,+1);
	glTexCoord2f(1,0); glVertex3f(+100,-100,+1);
	glTexCoord2f(1,1); glVertex3f(+100,+100,+1);
	glTexCoord2f(0,1); glVertex3f(-100,+100,+1);
	glEnd();
}

void show_lights() {
	float Emission[]  = {0.0,0.0,0.0,1.0};
	float Ambient[]   = {0.3,0.3,0.3,1.0};
	float Diffuse[]   = {1.0,1.0,1.0,1.0};
	float Specular[]  = {1.0,1.0,1.0,1.0};
	float Position[]  = {40*Cos(zh),Ylight,40*Sin(zh),1.0};
	float Shinyness[] = {16};
	
	glEnable(GL_LIGHTING);
	//  Enable light 0
	glEnable(GL_LIGHT0);
	//  Set ambient, diffuse, specular components and position of light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);
	//  Set materials
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
	
	glPushMatrix();
	glTranslatef(Position[0], Position[1], Position[2]);
	gluSphere(quad, 5, 5, 5);
	glPopMatrix();
	
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(cam.pos.x, cam.pos.y, cam.pos.z);
	glRotatef(x_rot, 1.0, 0.0, 0.0);
	glRotatef(y_rot, 0.0, 1.0, 0.0);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture( GL_TEXTURE_2D, 1 );
	
	//  Enable lighting
	show_lights();
	
	glUseProgram(prog);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	md5.Draw();
	glUseProgram(0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	//md5.Draw_Bounds(frame);
	//md5.Draw_Skeleton(frame);
	//md5.Draw_Vectors();
	SDL_GL_SwapBuffers();
}

void mouse_click(SDL_MouseButtonEvent* event) {
	if(event->type == SDL_MOUSEBUTTONDOWN) {
		if(event->button == SDL_BUTTON_LEFT)
			left_down = true;
		if(event->button == SDL_BUTTON_RIGHT) {
			right_down = true;
		}
	}
	if(event->type == SDL_MOUSEBUTTONUP) {
		if(event->button == SDL_BUTTON_LEFT)
			left_down = false;
		if(event->button == SDL_BUTTON_RIGHT) {
			right_down = false;
		}
	}
}

void mouse_move(SDL_MouseMotionEvent* event) {
	if ( left_down ) {	
		y_rot += event->xrel * 0.1;
		x_rot += event->yrel * 0.1;
	}

	if ( right_down ) {
		cam.pos.x += event->xrel * 0.1;
		cam.pos.z += event->yrel * 0.1;
	}
}

void handle_key(SDL_KeyboardEvent* event) {
	
	switch(event->type) {
		case SDL_KEYDOWN:
			std::cout << "key pressed: " << event->keysym.sym << std::endl;
			if(event->keysym.sym==SDLK_l) {
				std::cout << light_move << std::endl;
				light_move = !light_move;
			}
			break;
	}
}

int main(int argc, char* argv[]) {
	#ifdef __LINUX__
	glewInit();
	#endif
	try {
		init();

		SDL_Event event;
		Timer fps;
		while(1) {
			while( SDL_PollEvent( &event ) )
			{

				switch(event.type) {
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						mouse_click((SDL_MouseButtonEvent*)&event);
						break;
					case SDL_MOUSEMOTION:
						mouse_move((SDL_MouseMotionEvent*)&event);
						break;
					case SDL_KEYDOWN:
						handle_key((SDL_KeyboardEvent*)&event);
						break;
					case SDL_QUIT:
						exit(0);
				}
			}
			if(light_move==true)
				zh += 3.0;
			//std::cout << zh << std::endl;
			if(zh > 360.0) {
				zh = 0.0;
			}
			md5.Build(frame++);
			//std::cout << "frame: " << frame << std::endl;
			if(frame >= 120) frame = 0;
			display();
			if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
			{
				SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
			}
		}
		SDL_Quit();
	}
	catch (std::string& s) {
		std::cout << "unhandled exception: " << s << std::endl;
	}
	return 0;
}