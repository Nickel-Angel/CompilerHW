#pragma once
#ifndef _ANALYZER_OUT_H_
#define _ANALYZER_OUT_H_

#include <vector>

class vocabulary;

void out(vocabulary);
void out(vocabulary, std::vector<vocabulary>);

#endif // _ANALYZER_OUT_H_