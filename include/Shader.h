#pragma once
#ifdef _WIN32
	#include <windows.h>
	#include "GL\glee.h"
	#include "GL\glu.h"
#endif

#ifdef __APPLE__
	#include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
#else
	#include <GL/glew.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void ErrCheck(char* where);

void Fatal(const char* format , ...);
//
//  Read text file
//
char* ReadText(char *file);

//
//  Print Shader Log
//
void PrintShaderLog(int obj,char* file);

//
//  Print Program Log
//
void PrintProgramLog(int obj);

//
//  Create Shader
//
int CreateShader(GLenum type,char* file);

//
//  Create Shader Program
//
int CreateShaderProg(char* VertFile,char* FragFile);

//
//  Create Shader Program with Location Names
//
int CreateShaderProgLoc(char* VertFile,char* FragFile,char* Name[]);