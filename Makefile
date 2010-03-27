MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
    OPENGL_INC= -FOpenGL
    OPENGL_LIB= -framework OpenGL
    SDL_INC= `sdl-config --cflags`
    SDL_LIB= `sdl-config --libs` -framework SDL_image
else
    OPENGL_INC= -I/usr/X11R6/include
    OPENGL_LIB= -I/usr/lib64 -lGL -lGLU
    SDL_INC= `sdl-config --cflags`
    SDL_LIB= `sdl-config --libs`
endif

# object files have corresponding source files
ODIR= Debug
SRCDIR= src
OBJS= main.o MD5.o Mesh.o Tokenizer.o GLee.o Camera.o Engine.o Timer.o Shader.o Animation.o
_OBJ = $(patsubst %,$(ODIR)/%,$(OBJS))
CXX=g++
COMPILER_FLAGS= -Wall
INCLUDE= $(SDL_INC) $(OPENGL_INC) -I./include
LIBS= $(SDL_LIB) $(OPENGL_LIB)

EXEC= md5_viewer

$(EXEC): $(OBJS)
	$(CXX) $(COMPILER_FLAGS) -o $(EXEC) $(_OBJ) $(LIBS) 
	
%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $(ODIR)/$@ $< $(INCLUDE)

%.o: $(SRCDIR)/%.c
	$(CXX) -c $(COMPILER_FLAGS) -o $(ODIR)/$@ $< $(INCLUDE)
		
clean:
	rm -f $(EXEC) $(_OBJ)