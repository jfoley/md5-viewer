#pragma once
#ifdef _WIN32
	#include <windows.h>
	#include "GL\glee.h"
	#include "GL\glu.h"
	#include "sdl_image.h"
#endif

#ifdef __APPLE__
	#include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
	#include "SDL_image/SDL_image.h"
#endif

#ifdef __LINUX__
	#include <GL/glew.h>
#endif

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <SDL.h>
#include "MD5_Types.h"
#include "Shader.h"
class Mesh {
public:
	Mesh();

	unsigned int texture;
	unsigned int normal_map;
	void Draw();
	void Draw_Vectors();
	void Build_Vertices(Joint* joints);
	void Build_Normals();
	void Load_Textures();

private:
	Shader shader_prog;

	std::string shader;
	std::vector<Vertex> verts;
	std::vector<Triangle> tris;
	std::vector<Weight> weights;
	
	int num_verts;
	int num_tris;
	int num_weights;

	
	unsigned int load_texture(std::string filename);
	friend class MD5_Parser;
};