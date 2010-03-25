#include "Animation.h"
using namespace std;

void
Animation::BuildFrameSkeleton (const vector<Hierarchy_t> jointInfos,
		    const std::vector<Joint>& baseframe_joints,
		    const float *animFrameData,
		    Joint *skelFrame,
		    int num_joints)
{
  int i;

  for (i = 0; i < num_joints; ++i)
    {
      const Joint* baseJoint = &baseframe_joints[i];
      Vec3 animatedPos;
      Quat animatedOrient;
      int j = 0;

	  animatedPos = baseJoint->origin;
	  animatedOrient = baseJoint->quat;

      if (jointInfos[i].flags & 1) /* Tx */
	{
		animatedPos.x = animFrameData[jointInfos[i].start_index + j];
	  ++j;
	}

      if (jointInfos[i].flags & 2) /* Ty */
	{
		animatedPos.y = animFrameData[jointInfos[i].start_index + j];
	  ++j;
	}

      if (jointInfos[i].flags & 4) /* Tz */
	{
		animatedPos.z = animFrameData[jointInfos[i].start_index + j];
	  ++j;
	}

      if (jointInfos[i].flags & 8) /* Qx */
	{
		animatedOrient.x = animFrameData[jointInfos[i].start_index + j];
	  ++j;
	}

      if (jointInfos[i].flags & 16) /* Qy */
	{
		animatedOrient.y = animFrameData[jointInfos[i].start_index + j];
	  ++j;
	}

      if (jointInfos[i].flags & 32) /* Qz */
	{
		animatedOrient.z = animFrameData[jointInfos[i].start_index + j];
	  ++j;
	}

      /* Compute orient quaternion's w value */
      //Quat_computeW (animatedOrient);
	  animatedOrient.calculate_W();
      /* NOTE: we assume that this joint's parent has
	 already been calculated, i.e. joint's ID should
	 never be smaller than its parent ID. */
      Joint* thisJoint = &skelFrame[i];

	  int parent = jointInfos[i].parent_index;
	  thisJoint->parent_index = parent;
      thisJoint->name = jointInfos[i].name;

      /* Has parent? */
	if (thisJoint->parent_index < 0)
	{
		thisJoint->origin = animatedPos;
		thisJoint->quat = animatedOrient;
	}
	else
	{
		Joint *parentJoint = &skelFrame[parent];
		Vec3 rpos; /* Rotated position */

		/* Add positions */
		//Quat_rotatePoint (parentJoint->orient, animatedPos, rpos);
		rpos = parentJoint->quat * animatedPos;
		//thisJoint->pos[0] = rpos[0] + parentJoint->pos[0];
		//thisJoint->pos[1] = rpos[1] + parentJoint->pos[1];
		//thisJoint->pos[2] = rpos[2] + parentJoint->pos[2];
		thisJoint->origin = rpos + parentJoint->origin;

		/* Concatenate rotations */
		//Quat_multQuat (parentJoint->orient, animatedOrient, thisJoint->orient);
		thisJoint->quat = animatedOrient * parentJoint->quat;
		//thisJoint->quat.normalize();
	}
    }
}

void Animation::build_baseframe(int frame_num) {
	float* frame_pointer = &this->frame_data[frame_num * this->numAnimatedComponents];

	for(int i = 0; i < this->numJoints; i++) {
		Joint& j = baseframe_joints[i];
		j.name = hierarchy_list[i].name;
		j.parent_index = hierarchy_list[i].parent_index;

		int k = 0;
		if(hierarchy_list[i].flags & 1) {
			j.origin.x = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 2) {
			j.origin.y = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 4) {
			j.origin.z = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 8) {
			j.quat.x = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 16) {
			j.quat.y = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 32) {
			j.quat.z = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}


		Joint animated_joint = j;
		if(j.parent_index < 0) {
			animated_joints[i] = animated_joint;
			continue;
		}

		Joint parent_joint = animated_joints[animated_joint.parent_index];

		Quat qp = Quat(j.origin.x, j.origin.y, j.origin.z, 0.0);
		Quat result = parent_joint.quat * qp * parent_joint.quat.conjugate();
		animated_joint.origin = Vec3(result.x, result.y, result.z) + parent_joint.origin;
		//animated_joint.quat = animated_joint.quat * j.quat;
		animated_joint.quat.normalize();

		animated_joints[i] = animated_joint;
	}
}

void Animation::Build_Frame(int frame_num) {
	animated_joints.clear();
	animated_joints.resize(this->numJoints);

	//build_baseframe(frame_num);
	BuildFrameSkeleton(hierarchy_list, baseframe_joints, &frame_data[frame_num * this->numAnimatedComponents],
		&animated_joints[0], animated_joints.size());
	return;

	//animated_joints.clear();
	//animated_joints.reserve(baseframe_joints.size());

	float* frame_pointer = &this->frame_data[frame_num * this->numAnimatedComponents];
	for(size_t i = 0; i < baseframe_joints.size(); i++) {
		Joint& j = baseframe_joints[i];
		Vec3 pos = j.origin;
		Quat quat = j.quat;
		
		int k = 0;
		if(hierarchy_list[i].flags & 1) {
			pos.x = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 2) {
			pos.y = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 4) {
			pos.z = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 8) {
			quat.x = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 16) {
			quat.y = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		if(hierarchy_list[i].flags & 32) {
			quat.z = frame_pointer[hierarchy_list[i].start_index + k];
			++k;
		}

		quat.calculate_W();

		Joint animated_joint;
		animated_joint.parent_index = hierarchy_list[i].parent_index;

		if(animated_joint.parent_index < 0) {
			animated_joint.origin = pos;
			animated_joint.quat = quat;
		}
		else {
			Joint parent_joint = baseframe_joints[animated_joint.parent_index];
			animated_joint.origin = parent_joint.quat * pos + parent_joint.origin;
			animated_joint.quat = animated_joint.quat * quat;
			animated_joint.quat.normalize();
		}
		animated_joints.push_back(animated_joint);
	}
}

void Animation::Load(const char* filename) {
	cout << "load animation called with " << filename << endl;
	this->tokens = new Tokenizer;
	this->tokens->Load(filename);
	parse();
	this->tokens->Unload();
	delete this->tokens;
}

void Animation::parse() {
	string keyword = tokens->read_keyword();
	if(keyword.compare("MD5Version") != 0) {
		//exception
	}

	if(tokens->read_int() != 10) {
		//exception
	}

	Token token = tokens->get_token();
	while(token.t != Token_type::TOKEN_INVALID) {
		if(token.t != Token_type::TOKEN_KEYWORD) {
			//exception
		}
		
		if(token.str.compare("numJoints") == 0) {
			this->numJoints = tokens->read_int();
		}

		if(token.str.compare("numFrames") == 0) {
			this->numFrames = tokens->read_int();
		}
		
		if(token.str.compare("frameRate") == 0) {
			this->frameRate = tokens->read_int();
		}

		if(token.str.compare("numAnimatedComponents") == 0) {
			this->numAnimatedComponents = tokens->read_int();
		}

		if(token.str.compare("hierarchy") == 0) {
			read_hierarchy();
		}

		if(token.str.compare("bounds") == 0) {
			read_bounds();
		}
		
		if(token.str.compare("baseframe") == 0) {
			read_baseframe();
		}

		if(token.str.compare("frame") == 0) {
			read_frame();
		}

		token = tokens->get_token();
	}
	cout << "animation file is valid" << endl;
}

void Animation::read_hierarchy() {
	tokens->read_lbrace();
	for(int i = 0; i < this->numJoints; i++) {
		Hierarchy_t hierarchy;
		tokens->read_string(hierarchy.name);
		hierarchy.parent_index = tokens->read_int();
		hierarchy.flags = tokens->read_int();
		hierarchy.start_index = tokens->read_int();
		hierarchy_list.push_back(hierarchy);
	}
	tokens->read_rbrace();
}

void Animation::read_bounds() {
	tokens->read_lparen();
	for(int i = 0; i < this->numJoints; i++) {
		Bounds_t bounds;
		tokens->read_vec3(bounds.min);
		tokens->read_vec3(bounds.max);
		bounds_list.push_back(bounds);
	}
	tokens->read_rparen();
}

void Animation::read_baseframe() {
	tokens->read_lbrace();
	for(int i = 0; i < this->numJoints; i++) {
		Joint j;
		j.name = hierarchy_list[i].name;
		j.parent_index = hierarchy_list[i].parent_index;
		tokens->read_vec3(j.origin);
		tokens->read_quat(j.quat);
		this->baseframe_joints.push_back(j);
	}
	tokens->read_rbrace();
}

void Animation::read_frame() {
	int frame_num = tokens->read_int();
	tokens->read_lbrace();
	for(int i = 0; i < this->numAnimatedComponents; i++) {
		this->frame_data.push_back(tokens->read_float());
	}
	tokens->read_rbrace();
}