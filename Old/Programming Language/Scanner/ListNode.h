/*
River Aden, Phase 3: Evaluator
*/
#pragma once
#include "Tree.h"
class ListNode {
public:
	ListNode();
	~ListNode();
	Token data;
	ListNode* next;
};

ListNode::ListNode() {}
ListNode::~ListNode() {}