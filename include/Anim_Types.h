#pragma once
#include <vector>
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

class Baseframe_t {
public:
	Vec3 pos;
	Quat orient;
};

class Frame_t {
public:
	std::vector<float> frame_data;
};