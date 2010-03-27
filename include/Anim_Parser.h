#pragma once
#include "Tokenizer.h"
#include "Anim_Types.h"

class Animation;
class Anim_Parser : public Tokenizer {
public:
	Anim_Parser(Animation& anim);
	//~Anim_Parser();

	void Load(std::string filename);
	void Parse();

private:
	Animation& parent_anim;
	void read_hierarchy();
	void read_bounds();
	void read_baseframe();
	void read_frame();
};