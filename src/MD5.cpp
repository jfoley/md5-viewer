#include "MD5.h"
using namespace std;

//MD5::MD5() {
//
//}

//MD5::~MD5(void)
//{
//
//}

void MD5::Load(string filename)
{
	MD5_Parser parser(*this);
	parser.Load(filename);
	parser.Parse();
	for(int i = 0; i < this->meshes.size(); i++) {
		meshes[i].load_textures();
	}
}

void MD5::LoadAnimation(string filename) {
	Animation anim;
	anim.Load(filename.c_str());
	this->animations.push_back(anim);
	current_anim = &animations.back();
	current_frame = 0;
	anim_index = this->animations.size() - 1;
}

void MD5::Build(int frame) {
	for(size_t i = 0; i < 1; i++) {
		meshes[i].build_vertices(current_anim->Get_Frame(frame));
		meshes[i].build_normals();
	}
}

void MD5::Draw() {
	meshes[0].Draw();
}

void MD5::Draw_Skeleton(int frame) {
	//vector<Joint>& joints = this->joints;
	Joint* joints = this->animations[0].Get_Frame(frame);
	
	glBegin(GL_LINES);
	for(size_t i = 0; i < this->num_joints; i++) {
		Joint& j = joints[i];
		if(j.parent_index < 0) continue;
		glColor3f(1.0, 0.0, 0.0);
		if(i == 2) glColor3f(1.0, 1.0, 0.0);
		Joint& p = joints[j.parent_index];
		
		glVertex3f(j.origin.x, j.origin.y, j.origin.z);
		glVertex3f(p.origin.x, p.origin.y, p.origin.z);
	}
	glEnd();
}

void MD5::Draw_Vectors() {
	for(size_t i = 0; i < meshes.size(); i++) {
		meshes[i].Draw_Vectors();
	}
}

void MD5::Next_Frame() {
	current_frame++;
	if(current_frame >= this->current_anim->Get_numFrames())
		current_frame = 0;
	this->Build(current_frame);
}

void MD5::Next_Animation() {
	anim_index++;
	if(anim_index >= this->animations.size())
		anim_index = 0;
	this->current_anim = &this->animations[anim_index];
	current_frame = 0;
}