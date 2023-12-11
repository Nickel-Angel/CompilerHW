#pragma once
#ifndef _TRANSLATOR_ACTION_H_
#define _TRANSLATOR_ACTION_H_

#include <variant>
#include <string>

using std::variant;

using identifier = variant<std::string, int, double>;

void init_translator();
void push_terminal(int, identifier);
void production_action(int);

#endif // _TRANSLATOR_ACTION_H_