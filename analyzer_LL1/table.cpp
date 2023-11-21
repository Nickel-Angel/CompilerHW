#include "..\compile_scanner\label.h"
#include "table.h"

using namespace analyzer_label;

using std::vector;
using std::pair;

constexpr auto MAX_PRODUCTION = 16;

production productionTable[MAX_PRODUCTION];

vector<pair<vocabulary, int>> LL1Table[MAX_VN]; // (indexed vocabulary, production number)

void init_production_table() {
	vector<vocabulary> temp;

	// 0: S -> E;Sp
	temp.clear();
	temp.emplace_back(vocabulary(false, E));
	temp.emplace_back(vocabulary(true, SEM));
	temp.emplace_back(vocabulary(false, Sp));
	productionTable[0] = { vocabulary(false, S), temp };

	// 1: S -> ;
	temp.clear();
	temp.emplace_back(vocabulary(true, SEM));
	productionTable[1] = { vocabulary(false, S), temp };

	// 2: Sp -> S
	temp.clear();
	temp.emplace_back(vocabulary(false, S));
	productionTable[2] = { vocabulary(false, Sp), temp };

	// 3: Sp -> eps
	temp.clear();
	productionTable[3] = { vocabulary(false, Sp), temp };

	// 4: E -> TEp
	temp.clear();
	temp.emplace_back(vocabulary(false, T));
	temp.emplace_back(vocabulary(false, Ep));
	productionTable[4] = { vocabulary(false, E), temp };

	// 5: Ep -> ATEp
	temp.clear();
	temp.emplace_back(vocabulary(false, A));
	temp.emplace_back(vocabulary(false, T));
	temp.emplace_back(vocabulary(false, Ep));
	productionTable[5] = { vocabulary(false, Ep), temp };

	// 6: Ep -> eps
	temp.clear();
	productionTable[6] = { vocabulary(false, Ep), temp };

	// 7: T -> FTp
	temp.clear();
	temp.emplace_back(vocabulary(false, F));
	temp.emplace_back(vocabulary(false, Tp));
	productionTable[7] = { vocabulary(false, T), temp };

	// 8: Tp -> MFTp
	temp.clear();
	temp.emplace_back(vocabulary(false, M));
	temp.emplace_back(vocabulary(false, F));
	temp.emplace_back(vocabulary(false, Tp));
	productionTable[8] = { vocabulary(false, Tp), temp };

	// 9: Tp -> eps
	temp.clear();
	productionTable[9] = { vocabulary(false, Tp), temp };

	// 10: F -> (E)
	temp.clear();
	temp.emplace_back(vocabulary(true, LBU));
	temp.emplace_back(vocabulary(false, E));
	temp.emplace_back(vocabulary(true, RBU));
	productionTable[10] = { vocabulary(false, F), temp };

	// 11: F -> i
	temp.clear();
	temp.emplace_back(vocabulary(true, ID));
	productionTable[11] = { vocabulary(false, F), temp };

	// 12: A -> +
	temp.clear();
	temp.emplace_back(vocabulary(true, PL));
	productionTable[12] = { vocabulary(false, A), temp };

	// 13: A -> -
	temp.clear();
	temp.emplace_back(vocabulary(true, MI));
	productionTable[13] = { vocabulary(false, A), temp };

	// 14: M -> *
	temp.clear();
	temp.emplace_back(vocabulary(true, MU));
	productionTable[14] = { vocabulary(false, M), temp };

	// 15: M -> /
	temp.clear();
	temp.emplace_back(vocabulary(true, DI));
	productionTable[15] = { vocabulary(false, M), temp };

}

void init_LL1_table() {
	// S
	LL1Table[S].emplace_back(vocabulary(true, ID), 0); // i S -> E;Sp
	LL1Table[S].emplace_back(vocabulary(true, LBU), 0); // ( S -> E;Sp

	LL1Table[S].emplace_back(vocabulary(true, SEM), 1); // ; S -> ;

	// Sp
	LL1Table[Sp].emplace_back(vocabulary(true, ID), 2); // i Sp -> S
	LL1Table[Sp].emplace_back(vocabulary(true, LBU), 2); // ( Sp -> S
	LL1Table[Sp].emplace_back(vocabulary(true, SEM), 2); // ; Sp -> S

	LL1Table[Sp].emplace_back(vocabulary(true, FIN), 3); // # Sp -> eps

	// E
	LL1Table[E].emplace_back(vocabulary(true, ID), 4); // i E -> TEp
	LL1Table[E].emplace_back(vocabulary(true, LBU), 4); // ( E -> TEp

	// Ep
	LL1Table[Ep].emplace_back(vocabulary(true, PL), 5); // + Ep -> ATEp
	LL1Table[Ep].emplace_back(vocabulary(true, MI), 5); // - Ep -> ATEp

	LL1Table[Ep].emplace_back(vocabulary(true, RBU), 6); // ) Ep -> eps
	LL1Table[Ep].emplace_back(vocabulary(true, SEM), 6); // ; Ep -> eps

	// T
	LL1Table[T].emplace_back(vocabulary(true, ID), 7); // i T -> FTp
	LL1Table[T].emplace_back(vocabulary(true, LBU), 7); // ( T -> FTp

	// Tp
	LL1Table[Tp].emplace_back(vocabulary(true, PL), 9); // + Tp -> eps
	LL1Table[Tp].emplace_back(vocabulary(true, MI), 9); // - Tp -> eps

	LL1Table[Tp].emplace_back(vocabulary(true, MU), 8); // * Tp -> MFTp
	LL1Table[Tp].emplace_back(vocabulary(true, DI), 8); // / Tp -> MFTp

	LL1Table[Tp].emplace_back(vocabulary(true, RBU), 9); // ( Tp -> eps
	LL1Table[Tp].emplace_back(vocabulary(true, SEM), 9); // ; Tp -> eps 

	// F
	LL1Table[F].emplace_back(vocabulary(true, ID), 11); // i F -> i

	LL1Table[F].emplace_back(vocabulary(true, LBU), 10); // ( F -> (E)

	// A
	LL1Table[A].emplace_back(vocabulary(true, PL), 12); // + A -> +

	LL1Table[A].emplace_back(vocabulary(true, MI), 13); // - A -> -

	// M
	LL1Table[M].emplace_back(vocabulary(true, MU), 14); // * M -> *

	LL1Table[M].emplace_back(vocabulary(true, DI), 15); // / M -> /
}

void init_table() {
	init_production_table();
	init_LL1_table();
}

int find_productionNum(int labelNum, vocabulary indexed) {
	auto& subTable = LL1Table[labelNum];
	for (auto& [v, productionNumber] : subTable) {
		if (v == indexed) {
			return productionNumber;
		}
	}
	return -1;
}

production find_productionByNum(int productionNum) {
	return productionTable[productionNum];
}