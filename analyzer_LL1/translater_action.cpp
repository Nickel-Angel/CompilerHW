#include "..\compile_scanner\label.h"
#include "translater_action.h"
#include "analyzer_out.h"
#include "analyzer.h"

#include <cstdio>
#include <cstring>
#include <string>

using namespace analyzer_label;

using std::string;

constexpr auto MAX_STACK_SIZE = 100;

class identifierStack {
private:
	identifier Stack[MAX_STACK_SIZE];
	int stackTop = 0;

public:
	void push(identifier i) {
		Stack[stackTop] = i;
		++stackTop;
	}

	identifier top() {
		if (stackTop > 0) {
			return Stack[stackTop - 1];
		}
		return 0;
	}

	void pop() {
		if (stackTop > 0) {
			--stackTop;
		}
	}

	int size() {
		return stackTop;
	}

	bool isempty() {
		return stackTop == 0;
	}
};

class operatorStack {
private:
	int Stack[MAX_STACK_SIZE];
	int stackTop = 0;

public:
	void push(int operatorLabel) {
		Stack[stackTop] = operatorLabel;
		++stackTop;
	}

	int top() {
		if (stackTop > 0) {
			return Stack[stackTop - 1];
		}
		return 0;
	}

	void pop() {
		if (stackTop > 0) {
			--stackTop;
		}
	}

	bool isempty() {
		return stackTop == 0;
	}
};

identifierStack iStack;
operatorStack oStack;
int tempVarCount;

void init_translater() {
	tempVarCount = 0;
	while (!iStack.isempty()) {
		iStack.pop();
	}
	while (!oStack.isempty()) {
		oStack.pop();
	}
}

void printIdentifier(const identifier &i) {
	switch (i.index()) {
	case 0:
		printf("%s", std::get<0>(i));
		break;
	case 1:
		printf("%d", std::get<1>(i));
		break;
	case 2:
		printf("%lf", std::get<2>(i));
		break;
	}
}

void printQuadruple(int operatorLabel, const identifier &op1, 
	const identifier &op2, const identifier &res) {
	printf("(");
	out(vocabulary(true, operatorLabel));
	printf(",");
	printIdentifier(op1);
	printf(",");
	printIdentifier(op2);
	printf(",");
	printIdentifier(res);
	printf(")\n");
}

double identifierToDouble(identifier i) {
	int *x = std::get_if<int>(&i);
	if (x != nullptr) {
		return (double)*x;
	}
	double *y = std::get_if<double>(&i);
	if (y != nullptr) {
		return *y;
	}
	return 0;
}

template<typename T>
T calcByType(T op1, T op2, int idType) {
	switch (idType) {
	case PL:
		return op1 + op2;
		break;
	case MI:
		return op1 - op2;
		break;
	case MU:
		return op1 * op2;
		break;
	case DI:
		return op1 / op2;
		break;
	}
	return op1;
}

void generateQuadruple() {
	if (iStack.size() < 2) {
		return;
	}
	identifier op2 = iStack.top();
	iStack.pop();
	identifier op1 = iStack.top();
	iStack.pop();

	identifier res;
	int idType = oStack.top();
	string tempVar;

	if (op2.index() == 0 || op1.index() == 0) {
		tempVar = "#T" + std::to_string(tempVarCount);
		res = (char*)tempVar.c_str();
		++tempVarCount;
	} else if (op2.index() == 2 || op1.index() == 2) {
		res = calcByType(identifierToDouble(op1), identifierToDouble(op2), idType);
	} else {
		res = calcByType(std::get<int>(op1), std::get<int>(op2), idType);
	}
	printQuadruple(oStack.top(), op1, op2, res);
	oStack.pop();
	iStack.push(res);
}

void ostack_push(int idType) {
	oStack.push(idType);
}

void push_terminal(int idType, identifier id) {
	if (idType == ID) {
		iStack.push(id);
		return;
	}
	oStack.push(idType);
}

void productionAction(int productionNum, int idType, identifier id) {
	switch (productionNum) {
	case 5: // E' -> ATE'
		while (iStack.size() > 1 && (oStack.top() == PL || oStack.top() == MI || // oStack.top() is ostack
			oStack.top() == MU || oStack.top() == DI)) {
			generateQuadruple();
		}
		break;
	case 8: // T' -> MFT'
		while (iStack.size() > 1 && (oStack.top() == MU || oStack.top() == DI)) {
			generateQuadruple();
		}
		break;
	case 9: // T' -> eps
		while (iStack.size() > 1 && (oStack.top() == PL || oStack.top() == MI ||
			oStack.top() == MU || oStack.top() == DI)) {
			generateQuadruple();
		}
		break;
	case 11: // F -> i
		iStack.push(id);
		break;
	case 12: // A -> +
		ostack_push(idType);
		break;
	case 13: // A -> -
		ostack_push(idType);
		break;
	case 14: // M -> *
		ostack_push(idType);
		break;
	case 15: // M -> /
		ostack_push(idType);
		break;
	default:
		break;
	}
}