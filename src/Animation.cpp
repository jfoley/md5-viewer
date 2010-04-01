#include "Animation.h"
using namespace std;

void Animation::Load(string filename) {

	Anim_Parser parser(*this);
	parser.Load(filename);
	parser.Parse();
}

void Animation::build_frame(int frame_num) {
	vector<float>& frame_pointer = this->frames[frame_num].frame_data;
	for(int i = 0; i < this->numJoints; i++) {
		Baseframe_t& base_joint = this->baseframe[i];
		Vec3 pos = base_joint.pos;
		Quat orient = base_joint.orient;

		int k = 0;
		if(hierarchy[i].flags & 1) {
			pos.x = frame_pointer[hierarchy[i].start_index + k];
			++k;
		}

		if(hierarchy[i].flags & 2) {
			pos.y = frame_pointer[hierarchy[i].start_index + k];
			++k;
		}

		if(hierarchy[i].flags & 4) {
			pos.z = frame_pointer[hierarchy[i].start_index + k];
			++k;
		}

		if(hierarchy[i].flags & 8) {
			orient.x = frame_pointer[hierarchy[i].start_index + k];
			++k;
		}

		if(hierarchy[i].flags & 16) {
			orient.y = frame_pointer[hierarchy[i].start_index + k];
			++k;
		}

		if(hierarchy[i].flags & 32) {
			orient.z = frame_pointer[hierarchy[i].start_index + k];
			++k;
		}


		Joint animated_joint;
		animated_joint.name = hierarchy[i].name;
		animated_joint.parent_index = hierarchy[i].parent_index;

		if(animated_joint.parent_index >= 0) {
			Joint& parent_joint = animated_joints[animated_joint.parent_index];
			animated_joint.origin = parent_joint.quat * pos + parent_joint.origin;
			animated_joint.quat = parent_joint.quat * orient;
			animated_joint.quat.normalize();
		}
		else {
			animated_joint.origin = pos;
			animated_joint.quat = orient;
		}
		animated_joints.push_back(animated_joint);
	}
}

Joint* Animation::Get_Frame(int frame_num) {
	animated_joints.clear();
	animated_joints.reserve(this->numJoints);
	build_frame(frame_num);
	return &this->animated_joints[0]; 
}

int Animation::Get_numFrames() {
	return this->numFrames;
}