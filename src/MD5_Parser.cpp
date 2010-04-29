#include "MD5_Parser.h"
#include "MD5.h"
using namespace std;

MD5_Parser::MD5_Parser(MD5& md5) 
: parent_md5(md5) {

}

void MD5_Parser::Load(string filename) {
	Tokenizer::Load(filename);
}

void MD5_Parser::Parse() {
	parent_md5.joints.empty();
	parent_md5.meshes.empty();

	string keyword = read_keyword();
	if((keyword.compare("MD5Version") != 0) || (read_int() != 10)) {
		throw string("MD5 file does not begin with valid header");
	}

	//start parsing the file regularly
	keyword = read_keyword();
	while(!keyword.empty()) {
		if(keyword.compare("numJoints") == 0) {
			parent_md5.num_joints = read_int();
		}
		else if(keyword.compare("numMeshes") == 0) {
			parent_md5.num_meshes = read_int();
		}
		else if(keyword.compare("joints") == 0) {
			read_joints();
		}

		else if(keyword.compare("mesh") == 0) {
			read_mesh();
		}
		else if(keyword.compare("commandline") == 0) {
			//ignore this
			get_token();
		}
		else {
			stringstream error;
			error << "unexpected keyword: " << keyword << endl;
			throw error.str();
		}
		keyword = read_keyword();
	}
}

void MD5_Parser::read_joints() {
	if(parent_md5.num_joints <= 0) {
		//exception
	}
	read_lbrace();

	for(int i = 0; i < parent_md5.num_joints; i++) {
		Joint joint;
		read_string(joint.name);
		joint.parent_index = read_int();
		read_vec3(joint.origin);
		read_quat(joint.quat);
		if(joint.parent_index >= 0) {
			if(joint.parent_index >= parent_md5.num_joints) {
				//exception
			}
			parent_md5.joints[joint.parent_index].children.push_back(parent_md5.joints.size());
		}
		parent_md5.joints.push_back(joint);
	}

	read_rbrace();
}

void MD5_Parser::read_mesh() {
	if(parent_md5.num_meshes <= 0) {
		throw string("number of meshes has not been read yet");
	}
	read_lbrace();
	Mesh mesh;
	string keyword = read_keyword();
	int verts_read = -1;
	int tris_read = -1;
	int weights_read = -1;
	while(!keyword.empty()) {
		if(keyword.compare("shader") == 0) {
			read_string(mesh.shader);
		}
		else if(keyword.compare("numverts") == 0) {
			mesh.num_verts = read_int();
			verts_read = 0;
		}
		else if(keyword.compare("vert") == 0) {
			if(verts_read > mesh.num_verts) {
				throw string("too many verts read");
			}
			Vertex vert;
			read_vert(vert);
			mesh.verts.push_back(vert);
			verts_read++;
		}
		else if(keyword.compare("numtris") == 0) {
			mesh.num_tris = read_int();
			tris_read = 0;
		}
		else if(keyword.compare("tri") == 0) {
			if(tris_read > mesh.num_tris) {
				throw string("too many tris read");
			}
			Triangle tri;
			read_tri(tri);
			mesh.tris.push_back(tri);
			tris_read++;
		}
		else if(keyword.compare("numweights") == 0) {
			mesh.num_weights = read_int();
			weights_read = 0;
		}
		else if(keyword.compare("weight") == 0) {
			if(weights_read > mesh.num_weights) {
				throw string("too many weights read");
			}
			Weight weight;
			read_weight(weight);
			mesh.weights.push_back(weight);
			weights_read++;
		}

		else {
			stringstream error;
			error << "unexpected keyword when reading mesh: " << keyword << endl;
			throw error.str();
		}
		
		if(mesh.num_verts == verts_read && 
			mesh.num_tris == tris_read &&
			mesh.num_weights == weights_read)
			break;
		keyword = read_keyword();
	}
	read_rbrace();
	parent_md5.meshes.push_back(mesh);
};

void MD5_Parser::read_tri(Triangle& tri) {
	tri.index = read_int();
	tri.indices[0] = read_int();
	tri.indices[1] = read_int();
	tri.indices[2] = read_int();
}

void MD5_Parser::read_vert(Vertex& vert) {
	vert.index = read_int();
	read_st(vert);
	vert.weight_index = read_int();
	vert.weight_count = read_int();
}

void MD5_Parser::read_weight(Weight& weight) {
	weight.index = read_int();
	weight.joint = read_int();
	weight.bias = read_float();
	read_vec3(weight.pos);
}

void MD5_Parser::read_st(Vertex& vert) {
	read_lparen();

	vert.tex_coords.s = read_float();
	vert.tex_coords.t = read_float();

	read_rparen();
};