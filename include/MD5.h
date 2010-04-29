#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "Mesh.h"
#include "MD5_Parser.h"
#include "Animation.h"

class MD5
{
public:
	//MD5();
	//~MD5(void);
	void Load(std::string filename);
	void LoadAnimation(std::string filename);
	void Build(int frame);
	void Draw();
	void Draw_Vectors();
	void Draw_Skeleton(int frame);
	void Next_Frame();
	void Next_Animation();

protected:
	int anim_index;
	int current_frame;
	int num_joints;
	int num_meshes;
	std::vector<Joint> joints;
	std::vector<Mesh> meshes;
	std::vector<Animation> animations;
	
	Animation* current_anim;	
	friend class MD5_Parser;
};
