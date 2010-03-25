#pragma once
#include <iostream>
#include <fstream>
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

class Token {
public:
	Token_type::Enum t;
	std::string str;
};

class Tokenizer
{
public:
	Tokenizer(void);
	~Tokenizer(void);
	void Load(std::string filename);
	void Unload();
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

	inline std::string token_type(Token_type::Enum t) {
		switch(t) {
			case Token_type::TOKEN_KEYWORD:
				return "TOKEN_KEYWORD"; break;
			case Token_type::TOKEN_INT:
				return "TOKEN_INT"; break;
			case Token_type::TOKEN_FLOAT:
				return "TOKEN_FLOAT"; break;
			case Token_type::TOKEN_STRING:
				return "TOKEN_STRING"; break;
			case Token_type::TOKEN_LBRACE:
				return "TOKEN_LBRACE"; break;
			case Token_type::TOKEN_RBRACE:
				return "TOKEN_RBRACE"; break;
			case Token_type::TOKEN_LPAREN:
				return "TOKEN_LPAREN"; break;
			case Token_type::TOKEN_RPAREN:
				return "TOKEN_RPAREN"; break;
			case Token_type::TOKEN_INVALID:
				return "TOKEN_INVALID"; break;
			default:
				return "BAD TOKEN!";
		}
	}


protected:
		

	std::ifstream file;
	std::string filename;
	std::string find_string();
	void skip_whitespace();
	void skip_comments();
};
