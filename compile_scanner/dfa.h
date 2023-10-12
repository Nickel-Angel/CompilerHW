#pragma once
#ifndef _DFA_H_
#define _DFA_H_

#include <cstring>
#include <cctype>
#include <vector>

using std::vector;

constexpr int MAX_ALPHA_NUMBER = 128; // ascii

class DFANode {
protected:
	bool isTerminal;
	int nodeNum;
	int next[MAX_ALPHA_NUMBER];

public:
	DFANode();
	DFANode(bool, int);
	
	void SetTerminal(bool);
	void SetNodeNum(int);
	void SetNext(int, int);

	int GetNext(int);
	bool IsTerminal();
};

class DFA {
protected:
	int totalNode, startNum;
	vector<DFANode> nodeList;

	void SetNext(int, int, int);
	void SetTerminal(int, bool);

public:
	DFA(int, int);

	bool Query(char*);
};

#endif // _DFA_H_
