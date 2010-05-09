#include "Model_Manager.h"
using namespace boost::filesystem;
using namespace std;

void Model_Manager::Init() {
	path model_root_path("./models");
	for(directory_iterator iter(model_root_path); iter != directory_iterator(); ++iter) 
	{
		if(is_directory(iter->status())) {
			this->Load_Model(iter->path().string());
		}
	}
}

void Model_Manager::Load_Model(string path_string) {
	MD5* md5 = new MD5();
	path model_path(path_string);
	string mesh_path;
	vector<string> animation_paths;
	
	for(directory_iterator iter(model_path);iter != directory_iterator(); ++iter) 
	{
		if(iter->path().extension() == ".md5mesh") {
			mesh_path = iter->path().string();
		}
		if(iter->path().extension() == ".md5anim") {
			animation_paths.push_back(iter->path().string());
		}
	}
	
	md5->Load(mesh_path);
	
	for(int i = 0; i < animation_paths.size(); i++) {
			md5->LoadAnimation(animation_paths[i]);
	}
	models.push_back(md5);
}

MD5& Model_Manager::Get_Model(unsigned int index) {
	return models[index];
}