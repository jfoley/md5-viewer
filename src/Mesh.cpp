#include "Mesh.h"
using namespace std;

Mesh::Mesh() {
	string shader_name = string("bump");
	shader_prog.Load_Shader_Program(shader_name);	
}

void Mesh::Build_Normals() {
	for(size_t i = 0; i < tris.size(); i++) {
		Vec3& v1 = verts[tris[i].indices[0]].pos;
		Vec3& v2 = verts[tris[i].indices[1]].pos;
		Vec3& v3 = verts[tris[i].indices[2]].pos;
		
		Tex_coords& tc1 = verts[tris[i].indices[0]].tex_coords;
		Tex_coords& tc2 = verts[tris[i].indices[1]].tex_coords;
		Tex_coords& tc3 = verts[tris[i].indices[2]].tex_coords;

		Vec3 dv1 = v2 - v1;
		Vec3 dv2 = v3 - v1;
		
		Tex_coords dtc1 = tc2 - tc1;
		Tex_coords dtc2 = tc3 - tc1;
		
		Vec3 normal = cross(dv2, dv1);
		 
		float coef = 1.0 / (dtc1.s * dtc2.t - dtc2.s * dtc1.t);
		Vec3 tangent;
		tangent.x = coef * ((dv1.x * dtc2.t) + (dv2.x * -dtc1.t));
		tangent.y = coef * ((dv1.y * dtc2.t) + (dv2.y * -dtc1.t));
		tangent.z = coef * ((dv1.z * dtc2.t) + (dv2.z * -dtc1.t));
		
		Vec3 bitangent = cross(tangent, normal);
		
		verts[tris[i].indices[0]].n += normal;
		verts[tris[i].indices[1]].n += normal;
		verts[tris[i].indices[2]].n += normal;
		
		verts[tris[i].indices[0]].tangent += tangent;
		verts[tris[i].indices[1]].tangent += tangent;
		verts[tris[i].indices[2]].tangent += tangent;
	}

	for(size_t i = 0; i < verts.size(); i++) {
		verts[i].n.normalize();
		verts[i].tangent.normalize();
	}
}

void Mesh::Build_Vertices(Joint* joints) {
	for(size_t i = 0; i < verts.size(); i++) {
		Vertex& vert = verts[i];
		Vec3& vector = vert.pos;
		vector.x = vector.y = vector.z = 0.0;

		for(size_t j = 0; j < vert.weight_count; j++) {
			const Weight& weight = weights[vert.weight_index + j];
			const Joint& joint = joints[weight.joint];

			Vec3 result = joint.quat * weight.pos;
			vector += (joint.origin + result) * weight.bias;
		}
	}
}

void Mesh::Draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, texture );
	glActiveTexture(GL_TEXTURE1);
	glBindTexture( GL_TEXTURE_2D, normal_map );
	glEnable( GL_TEXTURE_2D );
	glColor3f(1.0, 1.0, 1.0);

	glUseProgram(shader_prog.Get_Program());
	int id = glGetUniformLocation(shader_prog.Get_Program(), "textureMap");
	if(id < 0)  {
		std::cout << " cant find colorMap! " << id << std::endl;
	}
	glUniform1i(id, 0);
	
	id = glGetUniformLocation(shader_prog.Get_Program(), "normalMap");
	if(id < 0)  {
		std::cout << "cant find normalMap! " << id << std::endl;
	}
	glUniform1i(id, 1);
	
	id = glGetAttribLocation(shader_prog.Get_Program(), "tangent");
	if(id < 0)  {
		std::cout << "cant find attrib! " << id << std::endl;
	}
	glBegin(GL_TRIANGLES);
	for(size_t i = 0; i < tris.size(); i++) {
		Triangle& tri = tris[i];
		for(int j = 0; j < 3; j++) {
			Vertex& vert = verts[tri.indices[j]];
			Vec3& point = vert.pos;
			Vec3& normal = vert.n;
			Vec3& tangent = vert.tangent;
			
			glVertexAttrib3f(id, tangent.x, tangent.y, tangent.z);
			glTexCoord2f(vert.tex_coords.s, vert.tex_coords.t);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);
		}
	}
	glEnd();
}

void Mesh::Draw_Vectors() {
	glBegin(GL_LINES);
	for(size_t i = 0; i < tris.size(); i++) {
		Triangle& tri = tris[i];
		for(int j = 0; j < 3; j++) {
			Vertex& vert = verts[tri.indices[j]];
			Vec3& point = vert.pos;
			Vec3& normal = vert.n;
			Vec3& tangent = vert.tangent;
			Vec3 binormal = cross(vert.n, vert.tangent);
			
			Vec3 n_v = point + normal;
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(point.x, point.y, point.z);
			glVertex3f(n_v.x, n_v.y, n_v.z);
			
			Vec3 t_v = point + tangent;
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(point.x, point.y, point.z);
			glVertex3f(t_v.x, t_v.y, t_v.z);
			
			Vec3 b_v = point + binormal;
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(point.x, point.y, point.z);
			glVertex3f(b_v.x, b_v.y, b_v.z);
		}
	}
	glEnd();
}

void Mesh::Load_Textures() {
	string texture_path = this->shader; texture_path.append(".tga");
	string normal_path = this->shader; normal_path.append("_local.tga");
	
	try {
		texture = load_texture(texture_path);
		normal_map = load_texture(normal_path);
	}
	catch(invalid_argument e) {
		cout << "Warning: could not load " << e.what() << endl;
	}
}

unsigned int Mesh::load_texture(std::string filename) {	
	SDL_Surface *surface = IMG_Load(filename.c_str());
	if(surface == NULL) {
		throw std::invalid_argument(filename);
	}

	GLenum texture_format;
	GLint  nOfColors;
	unsigned int image = 0;
	// Check that the image's width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		throw std::invalid_argument(filename.append(" width is not a power of 2."));
	}
	
	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		throw std::invalid_argument(filename.append(" height is not a power of 2."));
	}
	
    // get the number of channels in the SDL surface
    nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4)     // contains an alpha channel
    {
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
    } else if (nOfColors == 3)     // no alpha channel
    {
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
    } else {
		throw std::invalid_argument(filename.append(" is not truecolor."));
    }
	GLenum error;
    
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &image );
	error = glGetError();
	if(error != GL_NO_ERROR) {
		throw std::runtime_error(string((const char*)gluErrorString(error)));
	}
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, image );
	
	// Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
				 texture_format, GL_UNSIGNED_BYTE, surface->pixels );

	error = glGetError();
	if(error != GL_NO_ERROR) {
		throw std::runtime_error(string((const char*)gluErrorString(error)));
	}
	
	// Free the SDL_Surface only if it was successfully created
	if ( surface ) 
		SDL_FreeSurface( surface );
	return image;
}