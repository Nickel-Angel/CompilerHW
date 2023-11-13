#include "analyzer_out.h"
#include "analyzer.h"

using namespace analyzer_label;

using std::vector;

void out(vocabulary v) {
	if (v.isTerminal) {
		switch (v.labelNum) {
        case ID:
            printf("i");
            break;
        case PL:
            printf("+");
            break;
        case MI:
            printf("-");
            break;
        case MU:
            printf("*");
            break;
        case DI:
            printf("/");
            break;
        case LBU:
            printf("(");
            break;
        case RBU:
            printf(")");
            break;
        case SEM:
            printf(";");
            break;
        case FIN:
            printf("#");
            break;
		}
        return;
	}
    switch (v.labelNum) {
    case S:
        printf("S");
        break;
    case Sp:
        printf("S'");
        break;
    case E:
        printf("E");
        break;
    case Ep:
        printf("E'");
        break;
    case T:
        printf("T");
        break;
    case Tp:
        printf("T'");
        break;
    case F:
        printf("F");
        break;
    case A:
        printf("A");
        break;
    case M:
        printf("M");
        break;
    }
}

void out(vocabulary left, vector<vocabulary> right) {
    out(left);
    printf(" -> ");
    for (vocabulary v : right) {
        out(v);
    }
}