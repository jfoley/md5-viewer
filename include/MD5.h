#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "Mesh.h"
#include "Tokenizer.h"
#include "Animation.h"

class MD5
{
public:
	MD5();
	~MD5(void);
	void Load(std::string filename);
	void LoadAnimation(std::string filename);
	void Build(int frame);
	void Draw();
	void Draw_Vectors();
	void Draw_Skeleton();
protected:
	std::vector<Joint> joint_list;
	std::vector<Mesh> mesh_list;
	std::vector<Animation> animation_list;
	
	Animation* current_anim;
	


	void read_st(Vertex& vert);
	void read_vert(Vertex& vert);
	void read_tri(Triangle& tri);
	void read_weight(Weight& weight);
	

	int num_joints;
	int num_meshes;
	
	void read_joints();
	void read_mesh();

	
	//std::list<token_pair> token_list;
	void parse();
	Tokenizer tokens;
	
};
