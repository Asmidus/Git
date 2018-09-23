/*
River Aden, Phase 3: Evaluator
*/
#include "stdafx.h"
#include "Token.h"

Token::Token() {
	value = "";
	type = INVALID;
}

Token::~Token() {}

Token::Token(string s, Type t) {
	value = s;
	type = t;
}