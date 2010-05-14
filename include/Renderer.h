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
#include <string>
#include <SDL.h>
#include <math.h>
#include <stdexcept>
#include "Camera.h"
#include "Model_Manager.h"

class Renderer {
public:
	//Renderer();
	//~Renderer();

	void Init();
	void Display(Model_Manager& model_manager);

	void move_light();
	void show_skeleton();
	void show_vectors();
	inline Camera& camera() { return cam; }

private:
	void show_lights();
	int prog, vert, frag;
	GLUquadric* quad;
	
	float zh;        //  Light azimuth
	float Ylight;   //  Light elevation
	bool light_move;
	bool draw_skeleton;
	bool draw_vectors;
	Camera cam;
};