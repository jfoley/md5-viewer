#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "Vec3.h"
#include "Quat.h"

namespace Token_type {
	enum Enum{ 
		TOKEN_KEYWORD,
		TOKEN_INT,
		TOKEN_FLOAT,
		TOKEN_STRING,
		TOKEN_LBRACE,
		TOKEN_RBRACE,
		TOKEN_LPAREN,
		TOKEN_RPAREN,
		TOKEN_INVALID 
	};
}

class Tokenizer
{
protected:
	class Token {
	public:
		Token_type::Enum t;
		std::string str;
	};
public:
	Tokenizer(void);
	virtual ~Tokenizer(void);
	virtual void Load(std::string filename);
	virtual void Unload();

protected:
	Token get_token();
	float read_float();
	int read_int();
	void read_string(std::string& str);
	void read_lparen();
	void read_rparen();
	void read_lbrace();
	void read_rbrace();
	std::string read_keyword();
	void read_vec3(Vec3& vec);
	void read_quat(Quat& quat);
	std::ifstream file;
	std::string filename;
	std::string find_string();
	void skip_whitespace();
	void skip_comments();
	std::string token_type(Token_type::Enum t);
};
