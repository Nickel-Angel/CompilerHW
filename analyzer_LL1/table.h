#pragma once
#ifndef _TABLE_H_
#define _TABLE_H_

#include "analyzer.h"

#include <vector>

using production = std::pair<vocabulary, std::vector<vocabulary>>;

void init_table();

int find_productionNum(int labelNum, vocabulary indexed);
production find_productionByNum(int productionNum);

#endif // _TABLE_H_