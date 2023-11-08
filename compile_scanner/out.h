#pragma once
#ifndef _OUT_H_
#define _OUT_H_

#include <cstdio>
#include <cstring>
#include <variant>

void set_outMode(int);

void out(int, char*);
void out(int, int);
void out(int, double);
void setIgnore(bool);

bool getIgnore();
int getScanLabel();
std::variant<char*, int, double> getScanResult();

void report_error(char*, int);

#endif // _OUT_H_