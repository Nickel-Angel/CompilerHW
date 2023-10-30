#include "analyzer.h"
#include "scanner.h"

using std::vector;
using std::pair;

constexpr int MAX_VN = 7;

char filename[10] = "test.txt";

vector<pair<vocabulary, vector<vocabulary>>> LL1Table[MAX_VN];

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

vocabulary::vocabulary() {
	this->isTerminal = false;
	this->labelNum = 0;
}

vocabulary::vocabulary(bool isTerminal, int labelNum) {
	this->isTerminal = isTerminal;
	this->labelNum = labelNum;
}

void initTable() {
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
}

void main_analyzer() {
	
}

void start_analyze() {
	initTable();
	FILE* fp = nullptr, * resultFlow = nullptr;
	printf("Please input source file name:\n");
	scanf_s("%s", filename, 9);
	errno_t err = fopen_s(&fp, filename, "r");
	while (err != 0) {
		printf("Fail to Open the file, please check your file name.\n");
		scanf_s("%s", filename);
		err = fopen_s(&fp, filename, "r");
	}
	set_current_row(1);
	set_outMode(1);
	while (!feof(fp)) {
		if (main_scanner(fp)) {
			
		}
	}
	fclose(fp);
}