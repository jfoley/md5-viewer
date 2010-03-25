#include "Tokenizer.h"

#define IS_WHITESPACE(c) (' ' == c || '\t' == c || '\r' ==c || '\n' == c )

using namespace std;

Tokenizer::Tokenizer(void)
{
}

Tokenizer::~Tokenizer(void)
{
	Unload();
}

void Tokenizer::Load(string filename) {
	this->filename = filename;
	this->file.open(filename.c_str(), ios::binary | ios::in);
	if(!this->file.is_open()) {
		throw string("unable to open file: ").append(filename);
	}
}

void Tokenizer::Unload() {
	if(file.is_open()) {
		file.close();
	}
}

float Tokenizer::read_float() {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_FLOAT || token.t != Token_type::TOKEN_INT) {
		//exception
	}
	return atof(token.str.c_str());
}

int Tokenizer::read_int() {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_INT) {
		//exception
	}
	return atoi(token.str.c_str());
}

void Tokenizer::skip_whitespace() {
  char c;
  while ( !file.eof() ) {
    file.get(c);

    if ( !IS_WHITESPACE(c) ) {
      file.putback(c);
      return;
    }
  }
}

Token Tokenizer::get_token() {
	char c;
	Token token = {Token_type::TOKEN_INVALID, ""};
	while(!file.eof()) {
		file.get(c);
		
		//keywords
		if(isalpha(c)) {
			token.str += c;
			if(token.t == Token_type::TOKEN_INVALID) {
				token.t = Token_type::TOKEN_KEYWORD;
			}
			else {
				//exception
			}
		}

		//whitespaces
		if(IS_WHITESPACE(c)) {
			if(token.t == Token_type::TOKEN_KEYWORD || token.t == Token_type::TOKEN_INT || token.t == Token_type::TOKEN_FLOAT) {
				return token;
			}
		}

		//numbers
		if(isdigit(c) || c == '-') {
			token.str += c;
			if(token.t == Token_type::TOKEN_INVALID) 
				token.t = Token_type::TOKEN_INT;
			else {
				//exception
			}
		}
		
		//floats
		if(c == '.') {
			token.str += c;
			if(token.t == Token_type::TOKEN_INT) 
				token.t = Token_type::TOKEN_FLOAT;
			else {
				//exception
			}
		}

		// parens
		if(c == '(') { 
			token.str += c; 
			token.t = Token_type::TOKEN_LPAREN;
			return token; 
		}
		if(c == ')') { 
			token.str += c; 
			token.t = Token_type::TOKEN_RPAREN;
			return token; }
		
		//braces
		if(c == '{') { 
			token.str += c;
			token.t = Token_type::TOKEN_LBRACE;
			return token; 
		}
		if(c == '}') { 
			token.str += c; 
			token.t = Token_type::TOKEN_RBRACE;
			return token; 
		}

		//quotes
		if(c == '"') {
			//read in a string
			token.t = Token_type::TOKEN_STRING;
			token.str = find_string();
			return token;
		}

		//comments
		if(c == '/') {
			skip_comments();
			skip_whitespace();
		}
	}
	return token;
}

string Tokenizer::find_string() {
	char c;
	string ret_string = "";
	while(!file.eof()) {
		file.get(c);
		if(c == '"') break;
		ret_string += c;
	}
	return ret_string;
}

void Tokenizer::skip_comments() {
	char c;
	file.get(c);
	if(c != '/') {
		//exception
	}

	while(!file.eof() && c != '\n')
		file.get(c);
	file.putback(c);
}

string Tokenizer::read_keyword() {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_KEYWORD) {
		if(token.t == Token_type::TOKEN_INVALID) {
			return string("");
		}
		//exception
	}
	return token.str;
}

void Tokenizer::read_vec3(Vec3& vec) {
	bool swizzle = true;
	read_lparen();
	if(!swizzle) {
		vec.x = read_float();
		vec.y = read_float();
		vec.z = read_float();
	}
	else {
		vec.z = read_float();
		vec.x = read_float();
		vec.y = read_float();
	}

	read_rparen();
}

void Tokenizer::read_quat(Quat& quat) {
	Vec3 vec;
	read_vec3(vec);
	quat.assign(vec.x, vec.y, vec.z);
}

void Tokenizer::read_string(string& str) {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_STRING) {
		//exception
	}
	str = token.str;
}

void Tokenizer::read_lparen() {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_LPAREN) {
		//exception
	}
}

void Tokenizer::read_rparen() {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_RPAREN) {
		//exception
	}
}

void Tokenizer::read_lbrace() {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_LBRACE) {
		//exception
	}
}

void Tokenizer::read_rbrace() {
	Token token = get_token();
	if(token.t != Token_type::TOKEN_RBRACE) {
		//exception
	}
}
