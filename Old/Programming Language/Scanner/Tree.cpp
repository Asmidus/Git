/*
River Aden, Phase 3: Evaluator
*/
#include "stdafx.h"
#include "Tree.h"
#include <string>
#include <iostream>
#include <fstream>

Tree::Tree() {}

Tree::~Tree() {}

Tree::Tree(Token t, Tree* l, Tree* r) {
	data = t;
	leftNode = l;
	middleNode = NULL;
	rightNode = r;
}

Tree::Tree(Token t) {
	data = t;
	leftNode = NULL;
	middleNode = NULL;
	rightNode = NULL;
}

void Tree::print(ofstream& fout) {
	print(fout, 0);
}

void Tree::print(ofstream& fout, int s) {
	for (int i = 0; i < s; i++) {
		fout << "        ";
	}
	if (&data == nullptr) {
		fout << "ERROR, improper use of punctuation" << endl;
		return;
	}
	if (data.value == "if") {
		fout << "IF-STATEMENT" << endl;
	}
	else if (data.value == "while") {
		fout << "WHILE-LOOP" << endl;
	} else {
		switch (data.type) {
		case 0:
			fout << "IDENTIFIER ";
			break;
		case 1:
			fout << "BOOLEAN ";
			break;
		case 2:
			fout << "NUMBER ";
			break;
		case 3:
			fout << "PUNCTUATION ";
			break;
		case 4:
			fout << "KEYWORD ";
			break;
		default:
			fout << "INVALID ";
		}
		fout << data.value << endl;
	}
	if (leftNode != NULL) {
		leftNode->print(fout, s+1);
	}
	if (middleNode != NULL) {
		middleNode->print(fout, s+1);
	}
	if (rightNode != NULL) {
		rightNode->print(fout, s+1);
	}
}

void Tree::deleteTree() {
	if (this != NULL) {
		leftNode->deleteTree();
		rightNode->deleteTree();
		delete(this);
	}
}