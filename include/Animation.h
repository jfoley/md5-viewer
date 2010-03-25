#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Tokenizer.h"
#include "MD5_Types.h"

class Hierarchy_t {
public:
	std::string name;
	int parent_index;
	int flags;
	int start_index;
};

class Bounds_t {
public:
	Vec3 min, max;
};

class Animation {
	Tokenizer* tokens;
	int numFrames;
	int numJoints;
	int frameRate;
	int numAnimatedComponents;
	std::vector<Hierarchy_t> hierarchy_list;
	std::vector<Bounds_t> bounds_list;
	std::vector<float> frame_data;
	
public:
	//Animation();
	void Load(const char* filename);
	//~Animation();
	std::vector<Joint> baseframe_joints;
	void Build_Frame(int frame_num);
	void BuildFrameSkeleton (const std::vector<Hierarchy_t> jointInfos,
		    const std::vector<Joint>& baseframe_joints,
		    const float *animFrameData,
		    Joint *skelFrame,
		    int num_joints);
	void build_baseframe(int frame_num);
	std::vector<Joint> animated_joints;

private:
	void parse();
	void read_hierarchy();
	void read_bounds();
	void read_baseframe();
	void read_frame();
};
