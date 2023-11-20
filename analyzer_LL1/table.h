#pragma once
#ifndef _TABLE_H_
#define _TABLE_H_

#include "analyzer.h"

#include <vector>

using production = std::pair<vocabulary, std::vector<vocabulary>>;

void init_table();

production find_production(int, vocabulary);

#endif // _TABLE_H_