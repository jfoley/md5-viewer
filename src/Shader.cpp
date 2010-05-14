#include "Shader.h"
const char* SHADER_PATH = "./shaders/";
const int MAX_FILE_LENGTH = 1024 * 10;
using namespace std;

int CreateShader(GLenum type, string &source)
{
	int shader = glCreateShader(type);

	const char* source_ptr = source.c_str();
	const char** source_ptr_ptr = &source_ptr;

	const char** test = (const char**) &(source_ptr);
	glShaderSource(shader, 1, (const char**) &(source_ptr), NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::string log(length, ' ');
		glGetShaderInfoLog(shader, length, &length, &log[0]);
		throw std::logic_error(log);
		return -1;
	}
	return shader;
}

void Shader::Load_Shader_Program(std::string &name) {
	string vert = string(name).append(".vert");
	string frag = string(name).append(".frag");
	this->Load_Shader_Program(vert, frag);
}

void Shader::Load_Shader_Program(string &vert, string &frag) {
	string vert_path = string(SHADER_PATH).append(vert);
	string frag_path = string(SHADER_PATH).append(frag);

	string vert_source;
	string frag_source;
		
	vert_source = Read_Source(vert_path);
	frag_source = Read_Source(frag_path);


	vert_shader = CreateShader(GL_VERTEX_SHADER, vert_source);
	frag_shader = CreateShader(GL_FRAGMENT_SHADER, frag_source);

	prog = glCreateProgram();
    glAttachShader(prog, vert_shader);
    glAttachShader(prog, frag_shader);
    glLinkProgram(prog);
	GLint status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		GLint length;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);	
		std::string log(length, ' ');
		glGetProgramInfoLog(prog, length, &length, &log[0]);
		throw std::logic_error(log);
	}
}

GLuint Shader::Compile(GLuint type, string &source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)source.c_str(), NULL);
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

string Shader::Read_Source(string &filename) {
	ifstream shader_file(filename.c_str());
	if(!shader_file.is_open()) {
		throw std::invalid_argument(filename);
	}
	string shader_source;

	shader_file.seekg(0, ios::end);   
	shader_source.reserve(shader_file.tellg());
	shader_file.seekg(0, ios::beg);

	shader_source.assign((istreambuf_iterator<char>(shader_file)),
						  istreambuf_iterator<char>());
	return shader_source;
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