#include "dfa.h"

#include <cstring>
#include <cctype>

// DFANode

DFANode::DFANode() {
	this->isTerminal = false;
	this->nodeNum = 0;
	memset(this->next, -1, sizeof(this->next));
}

DFANode::DFANode(bool isTerminal, int nodeNum) {
	this->isTerminal = isTerminal;
	this->nodeNum = nodeNum;
	memset(this->next, -1, sizeof(this->next));
}

void DFANode::SetTerminal(bool isTerminal) {
	this->isTerminal = isTerminal;
}

void DFANode::SetNodeNum(int nodeNum) {
	this->nodeNum = nodeNum;
}

void DFANode::SetNext(int nextNode, int alphaLabel) {
	this->next[alphaLabel] = nextNode;
}

int DFANode::GetNext(int alphaLabel) {
	return this->next[alphaLabel];
}

bool DFANode::IsTerminal() {
	return this->isTerminal;
}

// DFA

DFA::DFA(int totalNode, int startNum = 0) {
	this->totalNode = totalNode;
	this->startNum = startNum;
	this->nodeList = vector<DFANode>(totalNode);
	for (int i = 0; i < totalNode; ++i) {
		this->nodeList[i].SetNodeNum(i);
	}
}

void DFA::SetNext(int u, int v, int alphaLabel) {
	this->nodeList[u].SetNext(v, alphaLabel);
}

void DFA::SetTerminal(int u, bool isTerminal) {
	this->nodeList[u].SetTerminal(isTerminal);
}

bool DFA::Query(char* token) {
	int length = strlen(token), u = this->startNum;
	if (u == -1) {
		return false;
	}
	for (int i = 0; i < length; ++i) {
		u = this->nodeList[u].GetNext(token[i]);
		if (u == -1) {
			return false;
		}
	}
	return this->nodeList[u].IsTerminal();
}