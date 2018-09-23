/*
River Aden, Phase 3: Evaluator
*/
#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <iterator>
#include "Token.h"
using namespace std;

vector<Token> scan(string input);
Type getType(string input);

string keyWords[] = { "if", "then", "else", "endif", "while", "do", "endwhile", "skip" };
string punctuation[] = { "+", "-", "*", "/", "&", "|", "!", "(", ")", "=", "==", ":=", ";" };
char invalid[] = { '@', '.', '\\', '\'', '^', '%', '$', '#', '"' };

vector<Token> scan(string input) {
	vector<Token> output;
	Token temp;
	string value;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] != ' ' && input[i] != '\t') {
			value += input[i];
		}
		if ((i < input.length() - 1 && !isalnum(input[i + 1])) || i == input.length() - 1 || !isalnum(input[i])) {
			if (i < input.length() - 1 && input[i] == '=' && input[i + 1] == '=') {
				value = "==";
				i++;
			}
			if (i < input.length() - 1 && input[i] == ':' && input[i + 1] == '=') {
				value = ":=";
				i++;
			}
			if (value != "") {
				temp.value = value;
				temp.type = getType(value);
				if (temp.type != INVALID) {
					output.push_back(temp);
				} else {
					output.push_back(temp);
					break;
				}
			}
			value = "";
		}
	}
	return output;
}

Type getType(string input) {
	bool isNum = true;
	bool isID = true;
	for (char c : input) {
		for (char err : invalid) {
			if (c == err) {
				return INVALID;
			}
		}
	}
	if (input == "false" || input == "true") {
		return BOOL;
	}
	for (string keyWord : keyWords) {
		if (input == keyWord) {
			return KEYWORD;
		}
	}
	for (char c : input) {
		if (!isdigit(c)) {
			isNum = false;
		}
	}
	if (isNum) {
		return NUMBER;
	}
	for (string punct : punctuation) {
		if (input == punct) {
			return PUNCTUATION;
		}
	}
	for (char c : input) {
		if (!isalnum(c)) {
			isID = false;
		}
	}
	if (isID) {
		return IDENTIFIER;
	}
	return INVALID;
}