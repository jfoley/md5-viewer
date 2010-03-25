#include "MD5.h"
using namespace std;

MD5::MD5() {

}

MD5::~MD5(void)
{

}

void MD5::Load(string filename)
{
	tokens.Load(filename);
	parse();
	tokens.Unload();
}

void MD5::Build(int frame) {
	for(size_t i = 0; i < mesh_list.size(); i++) {
		//this->animation_list[0].Build_Frame(frame);
		mesh_list[i].build_vertices(this->joint_list);
		mesh_list[i].build_normals();
	}
}


void MD5::LoadAnimation(string filename) {
	Animation anim;
	anim.Load(filename.c_str());
	this->animation_list.push_back(anim);
	current_anim = &animation_list.back();
}

void MD5::Draw() {
	for(size_t i = 0; i < mesh_list.size(); i++) {
		mesh_list[i].Draw();
	}
}

void MD5::Draw_Skeleton() {
	//vector<Joint>& joints = this->animation_list[0].animated_joints;
	vector<Joint>& joints = this->joint_list;
	
	glBegin(GL_LINES);
	for(size_t i = 0; i < joints.size(); i++) {
		Joint& j = joints[i];
		if(j.parent_index < 0) continue;
		glColor3f(1.0, 0.0, 0.0);
		if(i == 1) glColor3f(1.0, 1.0, 0.0);
		Joint& p = joints[j.parent_index];
		
		glVertex3f(j.origin.x, j.origin.y, j.origin.z);
		glVertex3f(p.origin.x, p.origin.y, p.origin.z);
	}
	glEnd();
}

void MD5::Draw_Vectors() {
	for(size_t i = 0; i < mesh_list.size(); i++) {
		mesh_list[i].Draw_Vectors();
	}
}

void MD5::parse() {
	string keyword = tokens.read_keyword();
	if(keyword.compare("MD5Version") != 0) {
		//exception
	}

	if(tokens.read_int() != 10) {
		//exception
	}

	//start parsing the file regularly
	keyword = tokens.read_keyword();
	while(!keyword.empty()) {
		
		if(keyword.compare("numJoints") == 0) {
			this->num_joints = tokens.read_int();
			this->joint_list.reserve(this->num_joints);
		}
		else if(keyword.compare("numMeshes") == 0) {
			this->num_meshes = tokens.read_int();
			this->mesh_list.reserve(this->num_meshes);
		}
		else if(keyword.compare("joints") == 0) {
			read_joints();
		}

		else if(keyword.compare("mesh") == 0) {
			//read the meshes
			read_mesh();
			return;
		}
		else if(keyword.compare("commandline") == 0) {
			//ignore this
			tokens.get_token();
		}
		else {
			//exception 
			cout << "bad keyword: " << keyword.c_str() << endl;
			exit(1);
		}
		keyword = tokens.read_keyword();
	}
}

void MD5::read_joints() {
	if(this->num_joints == 0) {
		//exception
	}
	Token token = tokens.get_token();
	if(token.t != Token_type::TOKEN_LBRACE) {
		//exception
	}

	for(int i = 0; i < this->num_joints; i++) {
		Joint joint;
		tokens.read_string(joint.name);
		joint.parent_index = tokens.read_int();
		tokens.read_vec3(joint.origin);
		tokens.read_quat(joint.quat);
		if(joint.parent_index >= 0) {
			if(joint.parent_index >= this->joint_list.size()) {
				//exception
			}
			this->joint_list[joint.parent_index].children.push_back(this->joint_list.size());
		}
		this->joint_list.push_back(joint);
	}

	token = tokens.get_token();
	if(token.t != Token_type::TOKEN_RBRACE) {
		//exception
	}
}

void MD5::read_mesh() {
	if(this->num_meshes <= 0) {
		//exception
	}
	Token token = tokens.get_token();
	if(token.t != Token_type::TOKEN_LBRACE) {
		//exception
	}
	
	Mesh mesh;
	while(token.t != Token_type::TOKEN_RBRACE) {
		token = tokens.get_token();
		string keyword = token.str;
		if(keyword.compare("shader") == 0) {
			tokens.read_string(mesh.shader);
			mesh.load_textures();
		}
		if(keyword.compare("numverts") == 0) {
			mesh.verts.reserve(tokens.read_int());
		}
		else if(keyword.compare("vert") == 0) {
			Vertex vert;
			read_vert(vert);
			mesh.verts.push_back(vert);
		}
		else if(keyword.compare("numtris") == 0) {
			mesh.tris.reserve(tokens.read_int());
		}
		else if(keyword.compare("tri") == 0) {
			Triangle tri;
			read_tri(tri);
			mesh.tris.push_back(tri);
		}
		else if(keyword.compare("numweights") == 0) {
			mesh.weights.reserve(tokens.read_float());
		}
		else if(keyword.compare("weight") == 0) {
			Weight weight;
			read_weight(weight);
			mesh.weights.push_back(weight);
		}
		else {
			//exception
		}
	}
	this->mesh_list.push_back(mesh);
};

void MD5::read_tri(Triangle& tri) {
	bool swizzle = true;
	tri.index = tokens.read_int();

	//backwards since they are in directx order
	if(swizzle) {
		tri.indices[0] = tokens.read_int();
		tri.indices[1] = tokens.read_int();
		tri.indices[2] = tokens.read_int();
	}
	else {
		tri.indices[2] = tokens.read_int();
		tri.indices[1] = tokens.read_int();
		tri.indices[0] = tokens.read_int();
	}
}

void MD5::read_vert(Vertex& vert) {
	vert.index = tokens.read_int();
	read_st(vert);
	vert.weight_index = tokens.read_int();
	vert.weight_count = tokens.read_int();
}

void MD5::read_weight(Weight& weight) {
	weight.index = tokens.read_int();
	weight.joint = tokens.read_int();
	weight.bias = tokens.read_float();
	tokens.read_vec3(weight.pos);
}

void MD5::read_st(Vertex& vert) {
	tokens.read_lparen();

	vert.tex_coords.s = tokens.read_float();
	vert.tex_coords.t = tokens.read_float();

	tokens.read_rparen();
};
