#pragma once
#include <iostream>
#include <vector>

#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/filesystem.hpp"
#include "MD5.h"

class Model_Manager {
public:
	void Init();
	void Load_Model(std::string path_string);
	MD5& Get_Model(unsigned int index);
	
private:
	boost::ptr_vector<MD5> models;
};
