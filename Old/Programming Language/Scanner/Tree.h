/*
River Aden, Phase 3: Evaluator
*/
#pragma once
#include <string>
#include "Token.h"

using namespace std;

class Tree {
public:
	Tree();
	Tree(Token, Tree*, Tree*);
	Tree(Token);
	~Tree();
	void deleteTree();
	void print(ofstream&);
	void print(ofstream&, int);

	Token data;
	Tree* leftNode;
	Tree* middleNode;
	Tree* rightNode;
};