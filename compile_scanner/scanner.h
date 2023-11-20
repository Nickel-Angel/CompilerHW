#pragma once
#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <cstdio>

#include "label.h"
#include "lookup.h"
#include "out.h"
#include "dfa.h"

bool main_scanner(FILE*);

void set_current_row(int);
int get_current_row();

#endif // _SCANNER_H_