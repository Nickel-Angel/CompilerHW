#pragma once
#ifndef _ANALYZER_ERROR_H_
#define _ANALYZER_ERROR_H_

#include "analyzer.h"
#include "translator_action.h"

void report_error(identifier);
void report_error(vocabulary, vocabulary, identifier);

#endif // _ANALYZER_ERROR_H_
