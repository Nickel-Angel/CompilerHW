#include "..\compile_scanner\scanner.h"
#include "analyzer.h"
#include "table.h"

#include <vector>
#include <variant>

using namespace analyzer_label;
using std::vector;
using std::pair;

constexpr int MAX_STACK_SIZE = 1000;

vector<pair<vocabulary, vector<vocabulary>>> LL1Table[MAX_VN];
vocabulary scannerLabelToAnalyzerLabel[MAX_SCANNER_LABEL_NUMBER + 1]; // start from 1
bool printProcess;

class analyzerStack {
private:
	vocabulary Stack[MAX_STACK_SIZE];
	int StackTop = 0;

public:
	void push(vocabulary v) {
		Stack[StackTop] = v;
		++StackTop;
	}

	void push(vector<vocabulary> production) {
		for (auto v = production.rbegin(); v != production.rend(); ++v) {
			push(*v);
		}
	}

	vocabulary top() {
		if (StackTop > 0) {
			return Stack[StackTop - 1];
		}
		return vocabulary();
	}

	void pop() {
		if (StackTop > 0) {
			--StackTop;
		}
	}

	bool isempty() {
		return StackTop == 0;
	}

	void print() {
		print(0);
	}

	void print(int limit) {
		for (int i = 0; i < StackTop; ++i) {
			out(Stack[i]);
			if (!Stack[i].isTerminal && (Stack[i].labelNum == Sp || Stack[i].labelNum == Ep || Stack[i].labelNum == Tp)) {
				--limit;
			}
		}
		for (int i = StackTop; i < limit; ++i) {
			putchar(' ');
		}
	}
};

analyzerStack Stack;

vocabulary::vocabulary() {
	this->isTerminal = false;
	this->labelNum = 0;
}

vocabulary::vocabulary(bool isTerminal, int labelNum) {
	this->isTerminal = isTerminal;
	this->labelNum = labelNum;
}

bool vocabulary::operator == (const vocabulary& rhs) const {
	return !this->isTerminal == !rhs.isTerminal && this->labelNum == rhs.labelNum;
}

void init_scanner() {
	set_current_row(1);
	set_outMode(1);
}

void init_analyzer_stack() {
	while (!Stack.isempty()) {
		Stack.pop();
	}
	Stack.push(vocabulary(true, FIN));
	Stack.push(vocabulary(false, S));
}

void init_label_switcher() {
	// ID INT REAL
	scannerLabelToAnalyzerLabel[scanner_label::ID] = vocabulary(true, ID);
	scannerLabelToAnalyzerLabel[scanner_label::INT] = vocabulary(true, ID);
	scannerLabelToAnalyzerLabel[scanner_label::REAL] = vocabulary(true, ID);

	// PL
	scannerLabelToAnalyzerLabel[scanner_label::PL] = vocabulary(true, PL);

	// MI
	scannerLabelToAnalyzerLabel[scanner_label::MI] = vocabulary(true, MI);

	// MU
	scannerLabelToAnalyzerLabel[scanner_label::MU] = vocabulary(true, MU);

	// DI
	scannerLabelToAnalyzerLabel[scanner_label::DI] = vocabulary(true, DI);

	// LBU
	scannerLabelToAnalyzerLabel[scanner_label::LBU] = vocabulary(true, LBU);

	// RBU
	scannerLabelToAnalyzerLabel[scanner_label::RBU] = vocabulary(true, RBU);

	// SEM
	scannerLabelToAnalyzerLabel[scanner_label::SEM] = vocabulary(true, SEM);
}

void report_error(std::variant<char*, int, double> scannerResult) {
	switch (scannerResult.index()) {
	case 0:
		printf("error occur around %s token ", std::get<0>(scannerResult));
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

bool main_analyzer(vocabulary indexed, std::variant<char*, int, double> scannerResult) {
	while (true) {
		if (Stack.top().isTerminal) {
			if (Stack.top() == indexed) {
				if (printProcess) { // process
					Stack.print(12);
					out(indexed);
					puts("");
				}

				Stack.pop();
				return true;
			}
			// To do: report errors
			report_error(scannerResult);
			return false;
		}

		production fitProduction = find_production(Stack.top().labelNum, indexed);
		
		if (fitProduction.first.labelNum == -1) {
			report_error(scannerResult);
			return false;
		}
		
		if (printProcess) { // process
			Stack.print(12);
			out(indexed);
			printf("           ");
			out(Stack.top(), fitProduction.second);
			puts("");
		}
		Stack.pop();
		Stack.push(fitProduction.second);
	}
}

void set_print_process(bool mode) {
	printProcess = mode;
}

void start_analyze(FILE* fp) {
	init_table();
	init_label_switcher();
	init_scanner();
	init_analyzer_stack(); // S will be put in the stack first
	bool isTotalSuccess = true;
	bool isCurrentSuccess = true;
	if (printProcess) {
		printf("Stack  Current Label  Used Production\n"); // process table head
	}
	while (true) {
		if (main_scanner(fp)) {
			if (feof(fp)) {
				isCurrentSuccess &= main_analyzer(vocabulary(true, FIN), (char*)"final #");
				isTotalSuccess &= isCurrentSuccess;
				break;
			}
			if (get_ignore()) {
				continue;
			}
			if (!isCurrentSuccess) {
				if (get_scan_label() == scanner_label::SEM) {
					isCurrentSuccess = true;
					init_analyzer_stack();
				}
				continue;
			}
			isCurrentSuccess &= main_analyzer(scannerLabelToAnalyzerLabel[get_scan_label()], get_scan_result());
			isTotalSuccess &= isCurrentSuccess;
		}
	}
	if (isTotalSuccess) {
		puts("Success Analyze!");
	}
}