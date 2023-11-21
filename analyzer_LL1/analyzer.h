#pragma once
#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include "analyzer_out.h"

#include <cstdio>

struct vocabulary {
	bool isTerminal;
	int labelNum;

	bool operator == (const vocabulary& rhs) const;

	vocabulary();
	vocabulary(bool, int);
};

void set_print_process(bool);
void set_translater_process(bool);
void start_analyze(FILE*);

#endif // _ANALYZER_H_