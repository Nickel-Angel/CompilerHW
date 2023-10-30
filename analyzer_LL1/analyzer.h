#pragma once
#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <vector>
#include <utility>
#include <variant>

struct vocabulary {
	bool isTerminal;
	int labelNum;

	vocabulary();
	vocabulary(bool, int);
};

void start_analyze();

#endif // _ANALYZER_H_