#include "Shader.h"
const char* SHADER_PATH = "./shaders/";
const int MAX_FILE_LENGTH = 1024 * 10;
using namespace std;

int CreateShader(GLenum type,char* source)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   //char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   //free(source);
   //  Compile the shader
   glCompileShader(shader);
   //  Check for errors
   //PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

void Shader::Load_Shader_Program(const char *name) {
	string vert = string(name).append(".vert");
	string frag = string(name).append(".frag");
	this->Load_Shader_Program(vert.c_str(), frag.c_str());
}

void Shader::Load_Shader_Program(const char *vert, const char *frag) {
	string vert_path = string(SHADER_PATH).append(vert);
	string frag_path = string(SHADER_PATH).append(frag);

	ifstream vert_file = ifstream(vert_path.c_str());
	ifstream frag_file = ifstream(frag_path.c_str());

	vert_file.seekg(0, ios::end);
	int vert_length = vert_file.tellg();
	vert_file.seekg(0, ios::beg);

	frag_file.seekg(0, ios::end);
	int frag_length = frag_file.tellg();
	frag_file.seekg(0, ios::beg);

	char vert_buffer[MAX_FILE_LENGTH];
	char frag_buffer[MAX_FILE_LENGTH];

	vert_file.read(vert_buffer, vert_length);
	vert_file.read(frag_buffer, frag_length);
	vert_file.close();
	frag_file.close();

	//vert_shader = Compile(GL_VERTEX_SHADER, vert_buffer);
	//frag_shader = Compile(GL_FRAGMENT_SHADER, frag_buffer);

	vert_shader = CreateShader(GL_VERTEX_SHADER, vert_buffer);
	frag_shader = CreateShader(GL_VERTEX_SHADER, frag_buffer);

	prog = glCreateProgram();
    glAttachShader(prog, vert_shader);
    glAttachShader(prog, frag_shader);
    glLinkProgram(prog);
	GLint status;
	glGetShaderiv(prog, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		GLint length;
		glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &length);	
		std::string log(length, ' ');
		glGetShaderInfoLog(prog, length, &length, &log[0]);
		throw std::logic_error(log);
	}
}

GLuint Shader::Compile(GLuint type, char const *source) {
	GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        throw std::logic_error(log);
        return false;
    }
    return shader;
}

//void ErrCheck(char* where)
//{
//   int err = glGetError();
//   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
//}
//
//void Fatal(const char* format , ...)
//{
//   va_list args;
//   va_start(args,format);
//   vfprintf(stderr,format,args);
//   va_end(args);
//   exit(1);
//}
////
////  Read text file
////
//char* ReadText(char *file)
//{
//   int   n;
//   char* buffer;
//   //  Open file
//   FILE* f = fopen(file,"rt");
//   if (!f) Fatal("Cannot open text file %s\n",file);
//   //  Seek to end to determine size, then rewind
//   fseek(f,0,SEEK_END);
//   n = ftell(f);
//   rewind(f);
//   //  Allocate memory for the whole file
//   buffer = (char*)malloc(n+1);
//   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
//   //  Snarf the file
//   if (fread(buffer,n,1,f)!=0) Fatal("Cannot read %d bytes for text file %s\n",n+1,file);
//   buffer[n] = 0;
//   //  Close and return
//   fclose(f);
//   return buffer;
//}
//
////
////  Print Shader Log
////
//void PrintShaderLog(int obj,char* file)
//{
//   int len=0;
//   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
//   if (len>1)
//   {
//      int n=0;
//      char* buffer = (char *)malloc(len);
//      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
//      glGetShaderInfoLog(obj,len,&n,buffer);
//      fprintf(stderr,"%s:\n%s\n",file,buffer);
//      free(buffer);
//   }
//   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
//   if (!len) Fatal("Error compiling %s\n",file);
//}
//
////
////  Print Program Log
////
//void PrintProgramLog(int obj)
//{
//   int len=0;
//   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
//   if (len>1)
//   {
//      int n=0;
//      char* buffer = (char *)malloc(len);
//      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
//      glGetProgramInfoLog(obj,len,&n,buffer);
//      fprintf(stderr,"%s\n",buffer);
//   }
//   glGetProgramiv(obj,GL_LINK_STATUS,&len);
//   if (!len) Fatal("Error linking program\n");
//}
//
////
////  Create Shader
////

//
////
////  Create Shader Program
////
//int CreateShaderProg(char* VertFile,char* FragFile)
//{
//   //  Create program
//   int prog = glCreateProgram();
//   //  Create and compile vertex shader
//   if (VertFile)
//   {
//      int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
//      glAttachShader(prog,vert);
//   }
//   //  Create and compile fragment shader
//   if (FragFile)
//   {
//      int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
//      glAttachShader(prog,frag);
//   }
//   //  Link program
//   glLinkProgram(prog);
//   //  Check for errors
//   PrintProgramLog(prog);
//   //  Return name
//   return prog;
//}
//
////
////  Create Shader Program with Location Names
////
//int CreateShaderProgLoc(char* VertFile,char* FragFile,char* Name[])
//{
//   int k;
//   //  Create program
//   int prog = glCreateProgram();
//   //  Create and compile vertex shader
//   if (VertFile)
//   {
//      int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
//      glAttachShader(prog,vert);
//   }
//   //  Create and compile fragment shader
//   if (FragFile)
//   {
//      int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
//      glAttachShader(prog,frag);
//   }
//   //  Set names
//   for (k=0;Name[k];k++)
//      if (Name[k][0])
//         glBindAttribLocation(prog,k,Name[k]);
//   ErrCheck("CreateShaderProg");
//   //  Link program
//   glLinkProgram(prog);
//   //  Check for errors
//   PrintProgramLog(prog);
//   //  Return name
//   return prog;
//}