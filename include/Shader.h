#pragma once
#ifdef _WIN32
	#include <windows.h>
	#include "GL\glee.h"
	#include "GL\glu.h"
#endif

#ifdef __APPLE__
	#include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
#endif

#ifdef __LINUX__
	#include <GL/glew.h>
#endif

#include <string>
#include <iostream>
#include <fstream>

class Shader {
public:
	void Load_Shader_Program(const char* name);
	void Load_Shader_Program(const char* vert, const char* frag);
	int Get_Program() const {return prog;}
private:
	GLuint prog;
	GLuint vert_shader, frag_shader;
	GLuint Compile(GLuint type, char const *source);
};

//void ErrCheck(char* where);
//
//void Fatal(const char* format , ...);
////
////  Read text file
////
//char* ReadText(char *file);
//
////
////  Print Shader Log
////
//void PrintShaderLog(int obj,char* file);
//
////
////  Print Program Log
////
//void PrintProgramLog(int obj);
//
////
////  Create Shader
////
//int CreateShader(GLenum type,char* file);
//
////
////  Create Shader Program
////
//int CreateShaderProg(char* VertFile,char* FragFile);
//
////
////  Create Shader Program with Location Names
////
//int CreateShaderProgLoc(char* VertFile,char* FragFile,char* Name[]);