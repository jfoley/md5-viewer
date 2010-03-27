#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Anim_Parser.h"

class Animation {
private:
	int numFrames;
	int numJoints;
	int frameRate;
	int numAnimatedComponents;
	std::vector<Hierarchy_t> hierarchy;
	std::vector<Bounds_t> bounds;
	std::vector<Frame_t> frames;
	std::vector<Baseframe_t> baseframe;
	std::vector<Joint> animated_joints;
	void build_frame(int frame_num);

public:
	//Animation();
	//~Animation();
	void Load(std::string filename);
	Joint* Get_Frame(int frame_num);
	int Get_numFrames();
	friend class Anim_Parser;
};
