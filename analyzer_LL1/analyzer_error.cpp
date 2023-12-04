#include "..\compile_scanner\scanner.h"
#include "..\compile_scanner\label.h"
#include "analyzer_error.h"
#include "analyzer.h"
#include "table.h"

using namespace analyzer_label;

void expect_identifier() {
	printf("expect a identifier or '(' ");
}

void expect_left_bracket() {
	printf("')' occur but there's no matched '(' ");
}

void expect_semicolon() {
	printf("expect a ';' ");
}

void expect_operator() {
	printf("expect a operator ");
}

void report_error(identifier scannerResult) {
	switch (scannerResult.index()) {
	case 0:
		printf("error occur around %s token ", std::get<0>(scannerResult).c_str());
		break;
	case 1:
		printf("error occur around INT %d ", std::get<1>(scannerResult));
		break;
	case 2:
		printf("error occur around REAL %lf ", std::get<2>(scannerResult));
		break;
	}
	printf("in line %d\n", get_current_row());
}

void report_error(vocabulary stackTop, vocabulary indexed, identifier scannerResult) {
	switch (stackTop.labelNum) {
	case S:
		if (indexed.labelNum == PL || indexed.labelNum == MI ||
			indexed.labelNum == MU || indexed.labelNum == DI) {
			expect_identifier();
			break;
		}
		if (indexed.labelNum == RBU) {
			expect_left_bracket();
			break;
		}
		if (indexed.labelNum == FIN) {
			expect_semicolon();
			break;
		}
		break;
	case Sp:
		if (indexed.labelNum == PL || indexed.labelNum == MI ||
			indexed.labelNum == MU || indexed.labelNum == DI) {
			expect_identifier();
			break;
		}
		if (indexed.labelNum == RBU) {
			expect_left_bracket();
			break;
		}
		break;
	case E:
		if (indexed.labelNum == PL || indexed.labelNum == MI ||
			indexed.labelNum == MU || indexed.labelNum == DI) {
			expect_identifier();
			break;
		}
		if (indexed.labelNum == RBU) {
			expect_left_bracket();
			break;
		}
		break;
	case Ep:
		if (indexed.labelNum == FIN) {
			expect_semicolon();
			break;
		}
	case Tp:
		if (indexed.labelNum == ID || indexed.labelNum == LBU) {
			expect_operator();
			break;
		}
		if (indexed.labelNum == FIN) {
			expect_semicolon();
			break;
		}
		break;
	default:
		report_error(scannerResult);
		break;
	}
	printf("in line %d\n", get_current_row());
}