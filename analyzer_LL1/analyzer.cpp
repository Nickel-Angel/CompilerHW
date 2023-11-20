#include "..\compile_scanner\scanner.h"
#include "analyzer.h"

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

	void push(vector<vocabulary> gen) {
		for (auto v = gen.rbegin(); v != gen.rend(); ++v) {
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

void init_table() {
	vector<vocabulary> temp;
	// S
	temp.emplace_back(vocabulary(false, E));
	temp.emplace_back(vocabulary(true, SEM));
	temp.emplace_back(vocabulary(false, Sp));
	LL1Table[S].emplace_back(vocabulary(true, ID), temp); // S i -> E;Sp
	LL1Table[S].emplace_back(vocabulary(true, LBU), temp); // S ( -> E;Sp

	temp.clear();
	temp.emplace_back(vocabulary(true, SEM));
	LL1Table[S].emplace_back(vocabulary(true, SEM), temp); // S ; -> ;

	// Sp
	temp.clear();
	temp.emplace_back(vocabulary(false, S));
	LL1Table[Sp].emplace_back(vocabulary(true, ID), temp); // Sp i -> S
	LL1Table[Sp].emplace_back(vocabulary(true, LBU), temp); // Sp ( -> S
	LL1Table[Sp].emplace_back(vocabulary(true, SEM), temp); // Sp ; -> S

	temp.clear();
	LL1Table[Sp].emplace_back(vocabulary(true, FIN), temp); // Sp # -> eps

	// E
	temp.clear();
	temp.emplace_back(vocabulary(false, T));
	temp.emplace_back(vocabulary(false, Ep));
	LL1Table[E].emplace_back(vocabulary(true, ID), temp); // E i -> TEp
	LL1Table[E].emplace_back(vocabulary(true, LBU), temp); // E ( -> TEp

	// Ep
	temp.clear();
	temp.emplace_back(vocabulary(false, A));
	temp.emplace_back(vocabulary(false, T));
	temp.emplace_back(vocabulary(false, Ep));
	LL1Table[Ep].emplace_back(vocabulary(true, PL), temp); // Ep + -> ATEp
	LL1Table[Ep].emplace_back(vocabulary(true, MI), temp); // Ep - -> ATEp

	temp.clear();
	LL1Table[Ep].emplace_back(vocabulary(true, RBU), temp); // Ep ) -> eps
	LL1Table[Ep].emplace_back(vocabulary(true, SEM), temp); // Ep ; -> eps

	// T
	temp.clear();
	temp.emplace_back(vocabulary(false, F));
	temp.emplace_back(vocabulary(false, Tp));
	LL1Table[T].emplace_back(vocabulary(true, ID), temp); // T i -> FTp
	LL1Table[T].emplace_back(vocabulary(true, LBU), temp); // T ( -> FTp

	// Tp
	temp.clear();
	LL1Table[Tp].emplace_back(vocabulary(true, PL), temp); // Tp + -> eps
	LL1Table[Tp].emplace_back(vocabulary(true, MI), temp); // Tp - -> eps

	temp.clear();
	temp.emplace_back(vocabulary(false, M));
	temp.emplace_back(vocabulary(false, F));
	temp.emplace_back(vocabulary(false, Tp));
	LL1Table[Tp].emplace_back(vocabulary(true, MU), temp); // Tp * -> MFTp
	LL1Table[Tp].emplace_back(vocabulary(true, DI), temp); // Tp / -> MFTp

	temp.clear();
	LL1Table[Tp].emplace_back(vocabulary(true, RBU), temp); // Tp ( -> eps
	LL1Table[Tp].emplace_back(vocabulary(true, SEM), temp); // Tp ; -> eps 

	// F
	temp.clear();
	temp.emplace_back(vocabulary(true, ID));
	LL1Table[F].emplace_back(vocabulary(true, ID), temp); // F i -> i

	temp.clear();
	temp.emplace_back(vocabulary(true, LBU));
	temp.emplace_back(vocabulary(false, E));
	temp.emplace_back(vocabulary(true, RBU));
	LL1Table[F].emplace_back(vocabulary(true, LBU), temp); // F ( -> (E)

	// A
	temp.clear();
	temp.emplace_back(vocabulary(true, PL));
	LL1Table[A].emplace_back(vocabulary(true, PL), temp); // A + -> +

	temp.clear();
	temp.emplace_back(vocabulary(true, MI));
	LL1Table[A].emplace_back(vocabulary(true, MI), temp); // A - -> -

	// M
	temp.clear();
	temp.emplace_back(vocabulary(true, MU));
	LL1Table[M].emplace_back(vocabulary(true, MU), temp); // M * -> *

	temp.clear();
	temp.emplace_back(vocabulary(true, DI));
	LL1Table[M].emplace_back(vocabulary(true, DI), temp); // M / -> /

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

void init_scanner() {
	set_current_row(1);
	set_outMode(1);
}

void init_stack() {
	while (!Stack.isempty()) {
		Stack.pop();
	}
	Stack.push(vocabulary(true, FIN));
	Stack.push(vocabulary(false, S));
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

bool main_analyzer(vocabulary v, std::variant<char*, int, double> scannerResult) {
	while (true) {
		if (Stack.top().isTerminal) {
			if (Stack.top() == v) {
				if (printProcess) { // process
					Stack.print(12);
					out(v);
					puts("");
				}

				Stack.pop();
				return true;
			}
			// To do: report errors
			report_error(scannerResult);
			return false;
		}
		bool find = false;
		auto& SubTable = LL1Table[Stack.top().labelNum];
		for (auto& [vt, gen] : SubTable) {
			if (vt == v) {
				if (printProcess) { // process
					Stack.print(12);
					out(v);
					printf("           ");
					out(Stack.top(), gen);
					puts("");
				}

				Stack.pop();
				Stack.push(gen);
				find = true;
				break;
			}
		}
		// To do: report errors
		if (!find) {
			report_error(scannerResult);
			return false;
		}
	}
}

void set_print_process(bool mode) {
	printProcess = mode;
}

void start_analyze(FILE* fp) {
	init_table();
	init_scanner();
	init_stack(); // S will be put in the stack first
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
					init_stack();
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