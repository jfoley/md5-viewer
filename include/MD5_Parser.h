#pragma once
#include "Tokenizer.h"
#include "MD5_Types.h"

class MD5;
class MD5_Parser : public Tokenizer {
public:
	MD5_Parser(MD5& md5);
	//~MD5_Parser();

	void Load(std::string filename);
	void Parse();

private:
	MD5& parent_md5;
	void read_st(Vertex& vert);
	void read_vert(Vertex& vert);
	void read_tri(Triangle& tri);
	void read_weight(Weight& weight);
	void read_joints();
	void read_mesh();
};
