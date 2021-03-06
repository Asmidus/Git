/*
River Aden, Phase 3: Evaluator
*/
#include "stdafx.h"
#include "Parser.h"
#include <map>

using namespace std;
map <string, int> numbers;
map <string, int>::iterator numIter;
map <string, bool> bools;
map <string, bool>::iterator boolIter;
bool error = false;

void evaluate(Tree* head);
void evaluateKeyword(Tree* head);
void evaluateAssignment(Tree* head);
bool evaluateBool(Tree* head);
int evaluateNumber(Tree* head);

void evaluate(Tree* head) {
	if (head == NULL) {
		return;
	}
	if (head->leftNode != NULL && (head->leftNode->data.type == PUNCTUATION || head->leftNode->data.type == KEYWORD) && head->data.type != KEYWORD) {
		evaluate(head->leftNode);
	}
	if (head->data.value == ":=") {
		evaluateAssignment(head);
	}
	if (head->data.type == KEYWORD) {
		evaluateKeyword(head);
	}
	if (head->rightNode != NULL && (head->rightNode->data.type == PUNCTUATION || head->rightNode->data.type == KEYWORD) && head->data.type != KEYWORD) {
		evaluate(head->rightNode);
	}
}

void evaluateKeyword(Tree* head) {
	if (head->data.value == "while") {
		while (evaluateBool(head->leftNode)) {
			evaluate(head->rightNode);
		}
	}
	if (head->data.value == "if") {
		if (evaluateBool(head->leftNode)) {
			evaluate(head->middleNode);
		} else {
			evaluate(head->rightNode);
		}
	}
}

void evaluateAssignment(Tree* head) {
	string check = head->rightNode->data.value;
	if (head->rightNode->data.type == BOOL || check == "==" || check == "&" || check == "|" || check == "=" || check == "!") {
		boolIter = bools.find(head->leftNode->data.value);
		if (boolIter != bools.end()) {
			bools.find(head->leftNode->data.value)->second = evaluateBool(head->rightNode);
		} else {
			bools.insert(pair <string, bool>(head->leftNode->data.value, evaluateBool(head->rightNode)));
		}
	} else {
		numIter = numbers.find(head->leftNode->data.value);
		if (numIter != numbers.end()) {
			numbers.find(head->leftNode->data.value)->second = evaluateNumber(head->rightNode);
		} else {
			numbers.insert(pair <string, int>(head->leftNode->data.value, evaluateNumber(head->rightNode)));
		}
	}
}

bool evaluateBool(Tree* head) {
	if (head->data.type == BOOL) {
		return(head->data.value == "true");
	}
	if (head->data.type == IDENTIFIER) {
		boolIter = bools.find(head->data.value);
		if (boolIter != bools.end()) {
			return(bools.find(head->data.value)->second);
		} else {
			cout << "Error, unknown identifier being used" << endl;
			error = true;
			return NULL;
		}
	}
	if (head->data.value == "==") {
		return(evaluateNumber(head->leftNode) == evaluateNumber(head->rightNode));
	}
	if (head->data.value == "=") {
		return(evaluateBool(head->leftNode) == evaluateBool(head->rightNode));
	}
	if (head->data.value == "!") {
		return(!evaluateBool(head->leftNode));
	}
	if (head->data.value == "|") {
		return(evaluateBool(head->leftNode) || evaluateBool(head->rightNode));
	}
	if (head->data.value == "&") {
		return(evaluateBool(head->leftNode) && evaluateBool(head->rightNode));
	}
}

int evaluateNumber(Tree* head) {
	if (head->data.type == NUMBER) {
		return(stoi(head->data.value));
	}
	if (head->data.type == IDENTIFIER) {
		numIter = numbers.find(head->data.value);
		if (numIter != numbers.end()) {
			return(numbers.find(head->data.value)->second);
		} else {
			cout << "Error, unknown identifier being used" << endl;
			error = true;
			return NULL;
		}
	}
	if (head->data.value == "-") {
		if (evaluateNumber(head->leftNode) < evaluateNumber(head->rightNode)) {
			return(0);
		}
		return(evaluateNumber(head->leftNode) - evaluateNumber(head->rightNode));
	}
	if (head->data.value == "+") {
		return(evaluateNumber(head->leftNode) + evaluateNumber(head->rightNode));
	}
	if (head->data.value == "*") {
		return(evaluateNumber(head->leftNode) * evaluateNumber(head->rightNode));
	}
	if (head->data.value == "/") {
		if (evaluateNumber(head->rightNode) != 0) {
			return(evaluateNumber(head->leftNode) / evaluateNumber(head->rightNode));
		}
	}
}

int main(int argc, char* argv[]) {
	vector<Token> list;
	vector<Token> temp;
	ifstream fin;
	ofstream fout;
	string line;
	ListNode* head = new ListNode();
	ListNode* tempNode = new ListNode();
	char c;
	if (argc == 3) {
		fin.open(argv[1]);
		fout.open(argv[2]);
	}
	else {
		cout << "Must specify input file and output file" << endl;
		return 1;
	}
	/*else {
		fin.open("input.txt");
		fout.open("output.txt");
	}*/
	if (fin.fail() || fout.fail()) return 5;
	while (!fin.eof()) {
		getline(fin, line);
		fout << line << endl;
		temp = scan(line);
		list.insert(list.end(), temp.begin(), temp.end());
		if (list[list.size() - 1].type == INVALID) {
			break;
		}
	}
	fout << endl;
	for (int i = 0; i < list.size(); i++) {
		if (i == 0) {
			head->data = list[i];
			head->next = tempNode;
		}
		else {
			tempNode->data = list[i];
			if (i < list.size() - 1) {
				tempNode->next = new ListNode();
				tempNode = tempNode->next;
			}
		}
	}
	iter = head;
	for (int i = 0; i < list.size(); i++) {
		fout << list[i].value;
		switch (list[i].type) {
		case IDENTIFIER:
			fout << ", IDENTIFIER" << endl;
			break;
		case BOOL:
			fout << ", BOOL" << endl;
			break;
		case NUMBER:
			fout << ", NUMBER" << endl;
			break;
		case PUNCTUATION:
			fout << ", PUNCTUATION" << endl;
			break;
		case KEYWORD:
			fout << ", KEYWORD" << endl;
			break;
		case INVALID:
			fout << ", ERROR" << endl;
			break;
		}
	}
	fout << endl;
	if (list[list.size() - 1].type != INVALID) {
		Tree* parse = parseStatement();
		parse->print(fout);
		evaluate(parse);
		parse->deleteTree();
	}
	fout << "Variables:" << endl;
	numIter = numbers.begin();
	while (numIter != numbers.end()) {
		fout << numIter->first << ": " << numIter->second << endl;
		numIter++;
	}
	boolIter = bools.begin();
	while (boolIter != bools.end()) {
		fout << boolIter->first << ": ";
		if (boolIter->second) {
			fout << "true" << endl;
		} else {
			fout << "false" << endl;
		}
		boolIter++;
	}
	if (error) {
		fout << "Unknown identifier used" << endl;
	}
	return 0;
}