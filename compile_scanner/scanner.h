#pragma once
#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <cstdio>
#include <cctype>
#include <cstring>
#include <string>
#include <variant>

#include "lookup.h"
#include "out.h"
#include "dfa.h"

bool main_scanner(FILE*);
void set_current_row(int);

#endif // _SCANNER_H_