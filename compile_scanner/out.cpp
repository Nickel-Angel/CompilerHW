#include <cstdio>
#include <cstring>

#include "out.h"

constexpr auto MAX_LABEL_NUMBER = 20;
constexpr char* Labels[MAX_LABEL_NUMBER] = {
    (char*)"BEGIN", (char*)"END", (char *)"CONST",
    (char*)"VAR", (char*)"IF", (char*)"ELSE",
    (char*)"ID", (char*)"INT", (char*)"REAL",
    (char*)"LT", (char*)"LE",  (char*)"EQ",
    (char*)"NE", (char*)"GT",  (char*)"GE",
    (char*)"IS", (char*)"PL",  (char*)"MI", 
    (char*)"MU", (char*)"DI"
};

void out(int labelNum, char* id) {
	printf("(%s, %s)\n", Labels[labelNum], id);
}

void out(int labelNum, int x) {
    printf("(%s, %d)\n", Labels[labelNum], x);
}

void out(int labelNum, double x) {
    printf("(%s, %lf)\n", Labels[labelNum], x);
}

void report_error(char* error) {
    printf("%s\n", error);
}
