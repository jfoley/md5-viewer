#include "Anim_Parser.h"
#include "Animation.h"
using namespace std;

Anim_Parser::Anim_Parser(Animation& anim)
:parent_anim(anim) {

}

void Anim_Parser::Load(string filename) {
	Tokenizer::Load(filename);
}

void Anim_Parser::Parse() {
	string keyword = read_keyword();
	if(keyword.compare("MD5Version") != 0) {
		//exception
	}

	if(read_int() != 10) {
		//exception
	}

	keyword = read_keyword();
	while(!keyword.empty()) {
		if(keyword.compare("commandline") == 0) {
			//ignore this
			get_token();
		}

		else if(keyword.compare("numJoints") == 0) {
			parent_anim.numJoints = read_int();
		}

		else if(keyword.compare("numFrames") == 0) {
			parent_anim.numFrames = read_int();
		}
		
		else if(keyword.compare("frameRate") == 0) {
			parent_anim.frameRate = read_int();
		}

		else if(keyword.compare("numAnimatedComponents") == 0) {
			parent_anim.numAnimatedComponents = read_int();
		}

		else if(keyword.compare("hierarchy") == 0) {
			read_hierarchy();
		}

		else if(keyword.compare("bounds") == 0) {
			read_bounds();
		}
		
		else if(keyword.compare("baseframe") == 0) {
			read_baseframe();
		}

		else if(keyword.compare("frame") == 0) {
			read_frame();
		}

		else {
			stringstream error;
			error << "unexpected keyword: " << keyword << endl;
			throw error.str();
		}
		keyword = read_keyword();
	}
}

void Anim_Parser::read_hierarchy() {
	read_lbrace();
	for(int i = 0; i < parent_anim.numJoints; i++) {
		Hierarchy_t hierarchy;
		read_string(hierarchy.name);
		hierarchy.parent_index = read_int();
		hierarchy.flags = read_int();
		hierarchy.start_index = read_int();
		parent_anim.hierarchy.push_back(hierarchy);
	}
	read_rbrace();
}

void Anim_Parser::read_bounds() {
	read_lbrace();
	for(int i = 0; i < parent_anim.numFrames; i++) {
		Bounds_t bounds;
		read_vec3(bounds.min);
		read_vec3(bounds.max);
		parent_anim.bounds.push_back(bounds);
	}
	read_rbrace();
}

void Anim_Parser::read_baseframe() {
	read_lbrace();
	for(int i = 0; i < parent_anim.numJoints; i++) {
		Baseframe_t base;
		read_vec3(base.pos);
		read_quat(base.orient);
		parent_anim.baseframe.push_back(base);
	}
	read_rbrace();
}

void Anim_Parser::read_frame() {
	int frame_num = read_int();
	read_lbrace();
	Frame_t frame;
	for(int i = 0; i < parent_anim.numAnimatedComponents; i++) {
		frame.frame_data.push_back(read_float());
	}
	read_rbrace();
	parent_anim.frames.push_back(frame);
}