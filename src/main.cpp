#ifdef _WIN32
	#include <windows.h>
	#include "GL\glee.h"
	#include "GL\glu.h"
#endif

#ifdef __APPLE__
	#include "CoreFoundation/CoreFoundation.h"
	#include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
#endif

#ifdef __LINUX__
	#include <GL/glew.h>
#endif

#include <fstream>
#include <string>
#include <exception>
#include <SDL.h>

#include "Model_Manager.h"
#include "MD5.h"
#include "Engine.h"
#include "Timer.h"
#include "Renderer.h"

Renderer renderer;
const int FRAMES_PER_SECOND = 30;
bool left_down = false;
bool right_down = false;

Model_Manager model_manager;
const char* MODEL_PATH = "models/hellknight";
const char* SHADER_PATH = "shaders";

using namespace std;

int init(void)
{
	renderer.Init();
	model_manager.Init();
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
		renderer.camera().rotate(Vec3(event->xrel * 0.1, event->yrel * 0.1, 0));
	}

	if ( right_down ) {
		renderer.camera().move(Vec3(event->xrel * 0.1, 0, event->yrel * 0.1));
	}
}

void handle_key(SDL_KeyboardEvent* event) {
	
	switch(event->type) {
		case SDL_KEYDOWN:
			std::cout << "key pressed: " << event->keysym.sym << std::endl;
			if(event->keysym.sym==SDLK_l) {
				renderer.move_light();
			}
			if(event->keysym.sym==SDLK_n) {
				model_manager.Get_Model(0).Next_Animation();
			}
			if(event->keysym.sym==SDLK_s) {
				renderer.show_skeleton();
			}
			if(event->keysym.sym==SDLK_v) {
				renderer.show_vectors();
			}
			break;
	}
}

int main(int argc, char* argv[]) {
	#ifdef __APPLE__    
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
		char path[PATH_MAX];
		if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
		{
			// error!
		}
		CFRelease(resourcesURL);
		
		chdir(path);
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
			
			renderer.Display(model_manager);
			model_manager.Get_Model(0).Next_Frame();
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