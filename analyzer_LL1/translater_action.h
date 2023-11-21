#pragma once
#ifndef _TRANSLATER_ACTION_H_
#define _TRANSLATER_ACTION_H_

#include <variant>

using std::variant;

using identifier = variant<char*, int, double>;

void init_translater();
void productionAction(int, int, identifier);

#endif // _TRANSLATER_ACTION_H_