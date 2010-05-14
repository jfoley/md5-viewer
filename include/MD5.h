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
	MD5();
	//~MD5(void);
	void Load(std::string filename);
	void LoadAnimation(std::string filename);
	void Build(int frame);
	void Draw_Skeleton();
	void Next_Frame();
	void Next_Animation();
	void Draw();
	void Draw_Vectors();

protected:
	
	inline Animation* current_anim() { return &animations[anim_index]; }
	int anim_index;
	int current_frame;
	int num_joints;
	int num_meshes;
	std::vector<Joint> joints;
	std::vector<Mesh> meshes;
	std::vector<Animation> animations;
		
private:
	MD5(MD5& copy);
	friend class MD5_Parser;
};
