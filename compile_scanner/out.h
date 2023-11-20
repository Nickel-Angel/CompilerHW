#pragma once
#ifndef _OUT_H_
#define _OUT_H_

#include <variant>

void set_outMode(int);

void out(int, char*);
void out(int, int);
void out(int, double);
void set_ignore(bool);

bool get_ignore();
int get_scan_label();
std::variant<char*, int, double> get_scan_result();

void report_error(char*, int);

#endif // _OUT_H_