#include "analyzer.h"

using std::vector;
using std::pair;

constexpr int MAX_VN = 7;
constexpr int MAX_STACK_SIZE = 1000;

vector<pair<vocabulary, vector<vocabulary>>> LL1Table[MAX_VN];
vocabulary scannerLabelToAnalyzerLabel[23];

enum VT {
	ID = 0,
	PL,
	MI,
	MU,
	DI,
	LBU,
	RBU,
	FIN
};

enum VN {
	E = 0,
	Ep,
	T,
	Tp,
	F,
	A,
	M
};

class ananlyzerStack {
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
		return Stack[StackTop];
	}

	void pop() {
		if (StackTop > 0) {
			--StackTop;
		}
	}
};

ananlyzerStack Stack;

vocabulary::vocabulary() {
	this->isTerminal = false;
	this->labelNum = 0;
}

vocabulary::vocabulary(bool isTerminal, int labelNum) {
	this->isTerminal = isTerminal;
	this->labelNum = labelNum;
}

bool vocabulary::operator == (const vocabulary& rhs) const {
	return this->isTerminal == rhs.isTerminal && this->labelNum == rhs.labelNum;
}

void init_table() {
	vector<vocabulary> temp;
	// E
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
	LL1Table[Ep].emplace_back(vocabulary(true, FIN), temp); // Ep # -> eps

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
	LL1Table[Tp].emplace_back(vocabulary(true, FIN), temp); // Tp # -> eps

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

	// ID
	scannerLabelToAnalyzerLabel[7] = vocabulary(true, ID);
	
	// PL
	scannerLabelToAnalyzerLabel[17] = vocabulary(true, PL);
	
	// MI
	scannerLabelToAnalyzerLabel[18] = vocabulary(true, MI);
	
	// MU
	scannerLabelToAnalyzerLabel[19] = vocabulary(true, MU);
	
	// DI
	scannerLabelToAnalyzerLabel[20] = vocabulary(true, DI);

	// LBU
	scannerLabelToAnalyzerLabel[21] = vocabulary(true, LBU);

	// RBU
	scannerLabelToAnalyzerLabel[22] = vocabulary(true, RBU);
}

void init_scanner() {
	set_current_row(1);
	set_outMode(1);
}

void init_stack() {
	Stack.push(vocabulary(true, FIN));
	Stack.push(vocabulary(false, E));
}

bool main_analyzer(vocabulary v) {
	if (Stack.top().isTerminal) {
		if (Stack.top() == v) {
			Stack.pop();
			return true;
		}
		// To do: report errors
		return false;
	}
	auto& SubTable = LL1Table[Stack.top().labelNum];
	for (auto& [vt, gen] : SubTable) {
		if (vt == v) {
			Stack.pop();
			Stack.push(gen);
			return true;
		}
	}
	// To do: report errors
	return false;
}

void start_analyze(FILE* fp) {
	init_table();
	init_scanner();
	init_stack();
	bool result = true;
	while (true) {
		if (main_scanner(fp)) {
			if (feof(fp)) {
				result &= main_analyzer(vocabulary(true, FIN));
				break;
			}
			result &= main_analyzer(scannerLabelToAnalyzerLabel[getScanLabel()]);
		}
	}
	if (result) {
		puts("Success Analyze!");
	}
}