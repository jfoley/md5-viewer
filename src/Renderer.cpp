#include "Renderer.h"

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
using namespace std;

void Renderer::Init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	if( SDL_SetVideoMode( 640, 480, 32, SDL_OPENGL ) < 0 )
    {
		exit(1);
    }
	SDL_WM_SetCaption( "MD5 Loader", NULL );

	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 0.01,  1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	quad = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	zh=0.0;
	Ylight=120;
}

void Renderer::show_lights() {
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
	if(light_move==true)
		zh += 3.0;
	if(zh > 360.0) {
		zh = 0.0;
	}

	light_move = true;
	draw_skeleton = false;
	draw_vectors = false;
}

void Renderer::Display(Model_Manager& model_manager) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(cam.pos.x, cam.pos.y, cam.pos.z);
	glRotatef(cam.rot.x, 1.0, 0.0, 0.0);
	glRotatef(cam.rot.y, 0.0, 1.0, 0.0);

	glColor3f(1.0, 1.0, 1.0);
	//  Enable lighting
	show_lights();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	if(draw_skeleton == true) {
		//glUseProgram(0);
		model_manager.Get_Model(0).Draw_Skeleton();	
	}
	else {
		model_manager.Get_Model(0).Draw();
	}
	glUseProgram(0);
	//glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);
	
	if(draw_vectors == true) {
		model_manager.Get_Model(0).Draw_Vectors();
	}	
	SDL_GL_SwapBuffers();
}

void Renderer::show_vectors() {
	draw_vectors = !draw_vectors;
}

void Renderer::show_skeleton() {
	draw_skeleton = !draw_skeleton;
}

void Renderer::move_light() {
	light_move = !light_move;
}