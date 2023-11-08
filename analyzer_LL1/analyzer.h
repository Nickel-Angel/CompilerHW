#pragma once
#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include "scanner.h"

#include <vector>
#include <utility>
#include <variant>

struct vocabulary {
	bool isTerminal;
	int labelNum;

	bool operator == (const vocabulary& rhs) const;

	vocabulary();
	vocabulary(bool, int);
};

void start_analyze(FILE*);

#endif // _ANALYZER_H_