/*
River Aden, Phase 3: Evaluator
*/
#pragma once
#include <string>

enum Type {
	IDENTIFIER,
	BOOL,
	NUMBER,
	PUNCTUATION,
	KEYWORD,
	INVALID
};

using namespace std;

class Token {
public:
	Token();
	Token(string, Type);
	~Token();

	string value;
	Type type;
};

