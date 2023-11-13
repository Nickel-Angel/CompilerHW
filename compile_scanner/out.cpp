#include "out.h"

constexpr char* Labels[MAX_SCANNER_LABEL_NUMBER] = {
    (char*)"BEGIN", (char*)"END", (char*)"CONST",
    (char*)"VAR", (char*)"IF", (char*)"ELSE",
    (char*)"ID", (char*)"INT", (char*)"REAL",
    (char*)"LT", (char*)"LE",  (char*)"EQ",
    (char*)"NE", (char*)"GT",  (char*)"GE",
    (char*)"IS", (char*)"PL",  (char*)"MI", 
    (char*)"MU", (char*)"DI",  (char*)"LBU",
    (char*)"RBU"
};

static bool ignore;
static int outMode = 0; // 0: stdout 1: load
static int preLabelNum;
static std::variant<char*, int, double> preResult;

void set_outMode(int mode) {
    if (mode == (outMode ^ 1)) {
        outMode = mode;
    }
}

void out(int labelNum, char* id) {
    if (outMode == 0) {
        printf("(%s, %s)\n", Labels[labelNum - 1], id);
        return;
    }
    preLabelNum = labelNum;
    preResult = id;
}

void out(int labelNum, int x) {
    if (outMode == 0) {
        printf("(%s, %d)\n", Labels[labelNum - 1], x);
        return;
    }
    preLabelNum = labelNum;
    preResult = x;
}

void out(int labelNum, double x) {
    if (outMode == 0) {
        printf("(%s, %g)\n", Labels[labelNum - 1], x);
        return;
    }
    preLabelNum = labelNum;
    preResult = x;
}

void set_ignore(bool x) {
    ignore = x;
}

bool get_ignore() {
    return ignore;
}

int get_scan_label() {
    return preLabelNum;
}

std::variant<char*, int, double> get_scan_result() {
    return preResult; // notice: the validation of the return value is UNCHECKED!!
}

void report_error(char* error, int currentRow) {
    printf("%s on line %d\n", error, currentRow);
}
