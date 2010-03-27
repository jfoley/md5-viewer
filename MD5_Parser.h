#pragma once
#include "Tokenizer.h"
#include "MD5_Types.h"

class MD5_Parser: public Tokenizer {
public:

	int Load(std::string filename);
	int parse();

	int num_joints;
	int num_meshes;
	void read_st(Vertex& vert);
	void read_vert(Vertex& vert);
	void read_tri(Triangle& tri);
	void read_weight(Weight& weight);
	void read_joints();
	void read_mesh();
};
