#pragma once
#include <vector>

#include "Vec3.h"
#include "Quat.h"

class Triangle {
public:
	int index;
	int indices[3];
};

class Tex_coords {
public:
	float s, t;
	Tex_coords() {
		s = t = 0.0;
	}
	
	Tex_coords(float new_s, float new_t) {
		s = new_s;
		t = new_t;
	}
};

Tex_coords inline operator-(Tex_coords const &lhs, Tex_coords const &rhs) {
	return Tex_coords(
		lhs.s - rhs.s,
		lhs.t - rhs.t
	);
}

class Weight {
public:
	int index;
	int joint;
	float bias;
	Vec3 pos;
};

class Vertex {
public:
	int index;
	Tex_coords tex_coords;
	int weight_index;
	unsigned int weight_count;
	Vec3 pos;
	Vec3 n;
	Vec3 tangent;
};

class Joint {
public:
	std::string name;
	int parent_index;
	Vec3 origin;
	Quat quat;
	std::vector<int> children;
	Joint() {
		name = "";
		parent_index = -2;
	}
};