#pragma once
#ifndef _OUT_H_
#define _OUT_H_

#include <variant>
#include <string>

void set_outMode(int);

void out(int, const char*);
void out(int, int);
void out(int, double);
void set_ignore(bool);

bool get_ignore();
int get_scan_label();
std::variant<std::string, int, double> get_scan_result();

void report_error(const char*, int);

#endif // _OUT_H_