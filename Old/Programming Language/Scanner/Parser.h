/*
River Aden, Phase 3: Evaluator
*/
#pragma once
#include "stdafx.h"
#include "Scanner.h"
#include "Tree.h"
#include "ListNode.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <iterator>

bool parseStatement(ListNode*& i);
bool parseBaseStatement(ListNode*& i);
bool parseAssignment(ListNode*& i);
bool parseIfStatement(ListNode*& i);
bool parseWhileStatement(ListNode*& i);
bool parseExpression(ListNode*& i);
bool parseNumExpression(ListNode*& i);
bool parseNumTerm(ListNode*& i);
bool parseNumFactor(ListNode*& i);
bool parseNumPiece(ListNode*& i);
bool parseNumElement(ListNode*& i);
bool parseBoolExpression(ListNode*& i);
bool parseBoolTerm(ListNode*& i);
bool parseBoolFactor(ListNode*& i);
bool parseBoolPiece(ListNode*& i);
bool parseBoolElement(ListNode*& i);
Tree* parseStatement();
Tree* parseBaseStatement();
Tree* parseAssignment();
Tree* parseIfStatement();
Tree* parseWhileStatement();
Tree* parseNumExpression();
Tree* parseNumTerm();
Tree* parseNumFactor();
Tree* parseNumPiece();
Tree* parseNumElement();
Tree* parseBoolExpression();
Tree* parseBoolTerm();
Tree* parseBoolFactor();
Tree* parseBoolPiece();
Tree* parseBoolElement();

ListNode* iter = NULL;

Tree* parseStatement() {
	ListNode* iterTemp = iter;
	if (iter == NULL || !parseStatement(iterTemp)) {
		return NULL;
	}
	Tree* newNode = parseBaseStatement();
	while (iter != NULL && iter->data.value == ";") {
		iter = iter->next;
		Tree* temp = parseBaseStatement();
		if (temp != NULL) {
			newNode = new Tree(Token(";", PUNCTUATION), newNode, temp);
		}
	}
	return newNode;
}

Tree* parseBaseStatement() {
	ListNode* iterTemp = iter;
	if (iter == NULL) {
		return NULL;
	}
	if (parseAssignment(iterTemp)) {
		return parseAssignment();
	}
	if (parseIfStatement(iterTemp)) {
		return parseIfStatement();
	}
	if (parseWhileStatement(iterTemp)) {
		return parseWhileStatement();
	}
	if (iter->data.value == "skip") {
		iter = iter->next;
		return new Tree(Token("skip", KEYWORD), NULL, NULL);
	}
	return NULL;
}

Tree* parseAssignment() {
	Tree* newNode = new Tree(iter->data);
	iter = iter->next->next;
	ListNode* iterTemp = iter;
	if (parseBoolExpression(iterTemp)) {
		newNode = new Tree(Token(":=", PUNCTUATION), newNode, parseBoolExpression());
	} else {
		newNode = new Tree(Token(":=", PUNCTUATION), newNode, parseNumExpression());
	}
	return newNode;
}

Tree* parseIfStatement() {
	iter = iter->next;
	Tree* newNode = new Tree(Token("if", KEYWORD), parseBoolExpression(), NULL);
	iter = iter->next;
	Tree* thenNode = parseStatement();
	iter = iter->next;
	Tree* elseNode = parseStatement();
	iter = iter->next;
	newNode->middleNode = thenNode;
	newNode->rightNode = elseNode;
	return newNode;
}

Tree* parseWhileStatement() {
	iter = iter->next;
	Tree* newNode = new Tree(Token("while", KEYWORD), parseBoolExpression(), NULL);
	iter = iter->next;
	Tree* doNode = parseStatement();
	newNode->rightNode = doNode;
	iter = iter->next;
	return newNode;
}

Tree* parseNumExpression() {
	Tree* newNode = parseNumTerm();
	while (iter->data.value == "+") {
		iter = iter->next;
		newNode = new Tree(Token("+", PUNCTUATION), newNode, parseNumTerm());
	}
	return newNode;
}

Tree* parseNumTerm() {
	Tree* newNode = parseNumFactor();
	while (iter->data.value == "-") {
		iter = iter->next;
		newNode = new Tree(Token("-", PUNCTUATION), newNode, parseNumFactor());
	}
	return newNode;
}

Tree* parseNumFactor() {
	Tree* newNode = parseNumPiece();
	while (iter->data.value == "/") {
		iter = iter->next;
		newNode = new Tree(Token("/", PUNCTUATION), newNode, parseNumPiece());
	}
	return newNode;
}

Tree* parseNumPiece() {
	Tree* newNode = parseNumElement();
	while (iter->data.value == "*") {
		iter = iter->next;
		newNode = new Tree(Token("*", PUNCTUATION), newNode, parseNumElement());
	}
	return newNode;
}

Tree* parseNumElement() {
	Tree* newNode = NULL;
	if (iter->data.type == NUMBER || iter->data.type == IDENTIFIER) {
		newNode = new Tree(iter->data);
		iter = iter->next;
	} else {
		iter = iter->next;
		newNode = parseNumExpression();
		iter = iter->next;
	}
	return newNode;
}

Tree* parseBoolExpression() {
	Tree* newNode = parseBoolTerm();
	if (iter->data.value == "=") {
		iter = iter->next;
		newNode = new Tree(Token("=", PUNCTUATION), newNode, parseBoolTerm());
	}
	return newNode;
}

Tree* parseBoolTerm() {
	Tree* newNode = parseBoolFactor();
	while (iter->data.value == "|") {
		iter = iter->next;
		newNode = new Tree(Token("|", PUNCTUATION), newNode, parseBoolFactor());
	}
	return newNode;
}

Tree* parseBoolFactor() {
	Tree* newNode = parseBoolPiece();
	while (iter->data.value == "&") {
		iter = iter->next;
		newNode = new Tree(Token("&", PUNCTUATION), newNode, parseBoolPiece());
	}
	return newNode;
}

Tree* parseBoolPiece() {
	Tree* newNode = NULL;
	if (iter->data.value == "!") {
		iter = iter->next;
		newNode = new Tree(Token("!", PUNCTUATION), parseBoolElement(), NULL);
	} else {
		newNode = parseBoolElement();
	}
	return newNode;
}

Tree* parseBoolElement() {
	ListNode* iterTemp = iter;
	Tree* newNode = NULL;
	if (parseNumExpression(iterTemp) && iterTemp->data.value == "==") {
		newNode = parseNumExpression();
		iter = iter->next;
		newNode = new Tree(Token("==", PUNCTUATION), newNode, parseNumExpression());
	} else if (iter->data.type == BOOL || iter->data.type == IDENTIFIER) {
		newNode = new Tree(iter->data);
		iter = iter->next;
	} else {
		iter = iter->next;
		newNode = parseBoolExpression();
		iter = iter->next;
	}
	return newNode;
}


bool parseStatement(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseBaseStatement(iterTemp)) {
		while (iterTemp != NULL && iterTemp->data.value == ";") {
			iterTemp = iterTemp->next;
			if (iterTemp != NULL && !parseBaseStatement(iterTemp) && iterTemp->data.type != KEYWORD) {
				return false;
			}
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseBaseStatement(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseAssignment(iterTemp) ^ parseIfStatement(iterTemp) ^ parseWhileStatement(iterTemp) ^ (iterTemp != NULL && iterTemp->data.value == "skip")) {
		if (iterTemp != NULL && iterTemp->data.value == "skip") {
			iterTemp = iterTemp->next;
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseAssignment(ListNode*& i) {
	ListNode* iterTemp = i;
	if (iterTemp != NULL && iterTemp->data.type == IDENTIFIER) {
		iterTemp = iterTemp->next;
		if (iterTemp->data.value == ":=") {
			iterTemp = iterTemp->next;
			if (parseExpression(iterTemp)) {
				i = iterTemp;
				return true;
			}
		}
	}
	return false;
}

bool parseIfStatement(ListNode*& i) {
	ListNode* iterTemp = i;
	if (iterTemp != NULL && iterTemp->data.value == "if") {
		iterTemp = iterTemp->next;
		if (parseBoolExpression(iterTemp)) {
			if (iterTemp != NULL && iterTemp->data.value == "then") {
				iterTemp = iterTemp->next;
				if (parseStatement(iterTemp)) {
					if (iterTemp != NULL && iterTemp->data.value == "else") {
						iterTemp = iterTemp->next;
						if (parseStatement(iterTemp)) {
							if (iterTemp != NULL && iterTemp->data.value == "endif") {
								iterTemp = iterTemp->next;
								i = iterTemp;
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool parseWhileStatement(ListNode*& i) {
	ListNode* iterTemp = i;
	if (iterTemp != NULL && iterTemp->data.value == "while") {
		iterTemp = iterTemp->next;
		if (parseBoolExpression(iterTemp)) {
			if (iterTemp != NULL && iterTemp->data.value == "do") {
				iterTemp = iterTemp->next;
				if (parseStatement(iterTemp)) {
					if (iterTemp != NULL && iterTemp->data.value == "endwhile") {
						iterTemp = iterTemp->next;
						i = iterTemp;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool parseExpression(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseBoolExpression(iterTemp) || parseNumExpression(iterTemp)) {
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseNumExpression(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseNumTerm(iterTemp)) {
		while (iterTemp != NULL && iterTemp->data.value == "+") {
			iterTemp = iterTemp->next;
			if (!parseNumTerm(iterTemp)) {
				return false;
			}
		}
		if (iterTemp != NULL && (iterTemp->data.value == "&" || iterTemp->data.value == "|" || iterTemp->data.value == "=")) {
			return false;
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseNumTerm(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseNumFactor(iterTemp)) {
		while (iterTemp != NULL && iterTemp->data.value == "-") {
			iterTemp = iterTemp->next;
			if (!parseNumFactor(iterTemp)) {
				return false;
			}
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseNumFactor(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseNumPiece(iterTemp)) {
		while (iterTemp != NULL && iterTemp->data.value == "/") {
			iterTemp = iterTemp->next;
			if (!parseNumPiece(iterTemp)) {
				return false;
			}
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseNumPiece(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseNumElement(iterTemp)) {
		while (iterTemp != NULL && iterTemp->data.value == "*") {
			iterTemp = iterTemp->next;
			if (!parseNumElement(iterTemp)) {
				return false;
			}
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseNumElement(ListNode*& i) {
	ListNode* iterTemp = i;
	if (iterTemp != NULL) {
		if (iterTemp->data.type == NUMBER || iterTemp->data.type == IDENTIFIER) {
			iterTemp = iterTemp->next;
			i = iterTemp;
			return true;
		}
		if (iterTemp->data.value == "(") {
			iterTemp = iterTemp->next;
			if (parseNumExpression(iterTemp)) {
				if (iterTemp->data.value == ")") {
					iterTemp = iterTemp->next;
					i = iterTemp;
					return true;
				}
			}
		}
	}
	return false;
}

bool parseBoolExpression(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseBoolTerm(iterTemp)) {
		if (iterTemp != NULL && iterTemp->data.value == "=") {
			iterTemp = iterTemp->next;
			if (!parseBoolTerm(iterTemp)) {
				return false;
			}
		}
		if (iterTemp != NULL && (iterTemp->data.value == "+" || iterTemp->data.value == "-" || iterTemp->data.value == "*" || iterTemp->data.value == "/")) {
			return false;
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseBoolTerm(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseBoolFactor(iterTemp)) {
		while (iterTemp != NULL && iterTemp->data.value == "|") {
			iterTemp = iterTemp->next;
			if (!parseBoolFactor(iterTemp)) {
				return false;
			}
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseBoolFactor(ListNode*& i) {
	ListNode* iterTemp = i;
	if (parseBoolPiece(iterTemp)) {
		while (iterTemp != NULL && iterTemp->data.value == "&") {
			iterTemp = iterTemp->next;
			if (!parseBoolPiece(iterTemp)) {
				return false;
			}
		}
		i = iterTemp;
		return true;
	}
	return false;
}

bool parseBoolPiece(ListNode*& i) {
	ListNode* iterTemp = i;
	if (iterTemp != NULL) {
		if (iterTemp->data.value == "!") {
			iterTemp = iterTemp->next;
			if (parseBoolElement(iterTemp)) {
				i = iterTemp;
				return true;
			}
		}
		if (parseBoolElement(iterTemp)) {
			i = iterTemp;
			return true;
		}
	}
	return false;
}

bool parseBoolElement(ListNode*& i) {
	ListNode* iterTemp = i;
	if (iterTemp != NULL) {
		if (iterTemp->data.type == BOOL) {
			iterTemp = iterTemp->next;
			i = iterTemp;
			return true;
		}
		if (parseNumExpression(iterTemp)) {
			if (iterTemp != NULL && iterTemp->data.value == "==") {
				iterTemp = iterTemp->next;
				if (parseNumExpression(iterTemp)) {
					i = iterTemp;
					return true;
				}
			}
		}
		iterTemp = i;
		if (iterTemp->data.type == IDENTIFIER) {
			iterTemp = iterTemp->next;
			i = iterTemp;
			return true;
		}
		if (iterTemp->data.value == "(") {
			iterTemp = iterTemp->next;
			if (parseBoolExpression(iterTemp)) {
				if (iterTemp->data.value == ")") {
					iterTemp = iterTemp->next;
					i = iterTemp;
					return true;
				}
			}
		}
	}
	return false;
}