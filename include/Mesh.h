#pragma once
#ifdef _WIN32
	#include <windows.h>
	#include "GL\glee.h"
	#include "GL\glu.h"
	#include "sdl_image.h"
#endif

#ifdef __APPLE__
	#include <SDL_image/SDL_image.h>
	#include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
#else
	#include "SDL/SDL_image.h"
	#include <GL/glew.h>
#endif

#include <string>
#include <vector>
#include <iostream>
#include "MD5_Types.h"

class Mesh {
public:
	std::string shader;
	std::vector<Vertex> verts;
	std::vector<Triangle> tris;
	std::vector<Weight> weights;
	unsigned int texture;
	unsigned int normal_map;
	void build_vertices(Joint* joints);
	void build_normals();
	void Draw();
	void Draw_Vectors();
	void load_textures();
	unsigned int load_texture(std::string filename);
	int num_verts;
	int num_tris;
	int num_weights;
};

extern int prog;