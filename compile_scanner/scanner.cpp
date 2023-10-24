#include "scanner.h"
#include "lookup.h"
#include "out.h"
#include "dfa.h"

enum ClassLabel {
    BEGIN = 1, // 1
    END,       // 2
    CONST,     // 3
    VAR,       // 4
    IF,        // 5
    ELSE,      // 6
    ID,        // 7
    INT,       // 8
    REAL,      // 9
    LT,        // 10
    LE,        // 11
    EQ,        // 12
    NE,        // 13
    GT,        // 14
    GE,        // 15
    IS,        // 16
    PL,        // 17
    MI,        // 18
    MU,        // 19
    DI         // 20
};

char TOKEN[20];

class RealDFA : public DFA {    
private:
    bool iNegative, pNegative;
    int preLabel, state; // 0: integer 1: decimal 2: power
    int integer, power, decimal, dot;
    void(RealDFA::*action[8][8])();

    void initState() {
        iNegative = false, pNegative = false;
        preLabel = 0, state = 0;
        integer = 0, power = 0, decimal = 0, dot = 0;
    }

    void addNewDigit() {
        switch (state) {
        case 0:
            integer = integer * 10 + preLabel - '0';
            break;
        case 1:
            decimal = decimal * 10 + preLabel - '0';
            ++dot;
            break;
        case 2:
            power = power * 10 + preLabel - '0';
            break;
        default:
            break;
        }
    }

    void changeNeg() {
        if (preLabel == '-') {
            if (state == 0) {
                iNegative = true;
            } else {
                pNegative = true;
            }
        }
    }

    void changeStateToDec() {
        state = 1;
    }

    void changeStateToPower() {
        state = 2;
    }

    void SetNumberNext(int u, int v) {
        for (int i = 0; i < 10; ++i) {
            SetNext(u, v, '0' + i);
        }
        action[u][v] = &RealDFA::addNewDigit;
    }

public:
    RealDFA() : DFA(8, 0) {
        initState();
        // 0
        SetNext(0, 1, '+');
        SetNext(0, 1, '-');
        action[0][1] = &RealDFA::changeNeg;
        SetNext(0, 4, '.');
        action[0][4] = &RealDFA::changeStateToDec;
        SetNumberNext(0, 2);
        // 1
        SetNumberNext(1, 2);
        SetNext(1, 4, '.');
        action[1][4] = &RealDFA::changeStateToDec;
        // 2
        SetNumberNext(2, 2);
        SetNext(2, 3, '.');
        action[2][3] = &RealDFA::changeStateToDec;
        SetNext(2, 5, 'e');
        SetNext(2, 5, 'E');
        action[2][5] = &RealDFA::changeStateToPower;
        SetTerminal(2, true);
        // 3
        SetNumberNext(3, 3);
        SetNext(3, 5, 'e');
        SetNext(3, 5, 'E');
        action[3][5] = &RealDFA::changeStateToPower;
        SetTerminal(3, true);
        // 4
        SetNumberNext(4, 3);
        // 5
        SetNumberNext(5, 7);
        SetNext(5, 6, '+');
        SetNext(5, 6, '-');
        action[5][6] = &RealDFA::changeNeg;
        // 6
        SetNumberNext(6, 7);
        // 7
        SetNumberNext(7, 7);
        SetTerminal(7, true);
    }

    bool Query(char* token, double &x) {
        initState();
        x = 0;
        int length = strlen(token), preNode = 0, curNode = 0;
        for (int i = 0; i < length; ++i) {
            preNode = curNode;
            curNode = nodeList[curNode].GetNext(token[i]);
            preLabel = token[i];
            if (curNode == -1) {
                return false;
            }
            (this->*(action[preNode][curNode]))();
        }
        if (!nodeList[curNode].IsTerminal())
            return false;
        x = decimal;
        for (int i = 0; i < dot; ++i) {
            x *= 0.1;
        }
        x += integer;
        for (int i = 0; i < power; ++i) {
            x *= pNegative ? 0.1 : 10;
        }
        if (iNegative) {
            x = -x;
        }
        return true;
    }
};

class IntDFA : public DFA {
private:
    bool Negative;
    int preLabel, base;
    int result;
    void(IntDFA::*action[6][6])();

    void initState() {
        Negative = false;
        preLabel = 0, base = 10;
        result = 0;
    }

    void addNewDigit() {
        if (base == 16) {
            result *= 16;
            if (isdigit(preLabel)) {
                result += preLabel - '0';
                return;
            }
            if (isupper(preLabel)) {
                result += preLabel - 'A';
                return;
            }
            if (islower(preLabel)) {
                result += preLabel - 'a';
                return;
            }
            return;
        }
        result = result * base + preLabel - '0';
    }

    void changeNeg() {
        if (preLabel == '-') {
            Negative = true;
        }
    }

    void changeBase() {
        if (preLabel == '0') {
            base = 8;
        } else {
            base = 16;
        }
    }

public:
    IntDFA() : DFA(6, 0) {
        initState();
        // 0
        SetNext(0, 1, '+');
        SetNext(0, 1, '-');
        action[0][1] = &IntDFA::changeNeg;
        for (int i = 1; i < 10; ++i) {
            SetNext(0, 2, '0' + i);
        }
        action[0][2] = &IntDFA::addNewDigit;
        SetNext(0, 3, '0');
        action[0][3] = &IntDFA::changeBase;
        // 1
        for (int i = 1; i < 10; ++i) {
            SetNext(1, 2, '0' + i);
        }
        action[1][2] = &IntDFA::addNewDigit;
        SetNext(1, 3, '0');
        action[1][3] = &IntDFA::changeBase;
        // 2
        for (int i = 0; i < 10; ++i) {
            SetNext(2, 2, '0' + i);
        }
        action[2][2] = &IntDFA::addNewDigit;
        SetTerminal(2, true);
        // 3
        for (int i = 0; i < 8; ++i) {
            SetNext(3, 5, '0' + i);
        }
        action[3][5] = &IntDFA::addNewDigit;
        SetNext(3, 4, 'x');
        action[3][4] = &IntDFA::changeBase;
        SetTerminal(3, true);
        // 4
        for (int i = 0; i < 10; ++i) {
            SetNext(4, 4, '0' + i);
        }
        for (int i = 0; i < 6; ++i) {
            SetNext(4, 4, 'a' + i);
            SetNext(4, 4, 'A' + i);
        }
        action[4][4] = &IntDFA::addNewDigit;
        SetTerminal(4, true);
        // 5
        for (int i = 0; i < 8; ++i) {
            SetNext(5, 5, '0' + i);
        }
        action[5][5] = &IntDFA::addNewDigit;
        SetTerminal(5, true);
    }

    bool Query(char* token, int& x) {
        initState();
        x = 0;
        int length = strlen(token), preNode = 0, curNode = 0;
        for (int i = 0; i < length; ++i) {
            preNode = curNode;
            curNode = nodeList[curNode].GetNext(token[i]);
            preLabel = token[i];
            if (curNode == -1) {
                return false;
            }
            (this->*(action[preNode][curNode]))();
        }
        if (!nodeList[curNode].IsTerminal())
            return false;
        x = result;
        if (Negative) {
            x = -x;
        }
        return true;
    }
};

RealDFA realDFA;
IntDFA intDFA;

void clearTOKEN() {
    int length = strlen(TOKEN);
    for (int i = 0; i < length; ++i) {
        TOKEN[i] = '\0';
    }
}

bool main_scanner(FILE* fp)
{
    static char ch = '\0';
    int i, c;
    if (ch == EOF) {
        ch = fgetc(fp);
        return true;
    }
    ch = fgetc(fp);
    if (isalpha(ch)) { /*it must be a identifer!*/
        TOKEN[0] = ch;
        ch = fgetc(fp);
        i = 1;
        while (isalnum(ch) && i < 19) {
            TOKEN[i] = ch;
            ++i;
            ch = fgetc(fp);
        }
        TOKEN[i] = '\0';
        fseek(fp, -1, 1); /*retract*/
        c = lookup(TOKEN);
        if (c == 0) {
            out(ID, TOKEN); // To do: identify the ID
        } else {
            out(c, (char*)" ");
        }
    } else if (isdigit(ch) || ch == '.') {
        TOKEN[0] = ch;
        ch = fgetc(fp);
        i = 1;
        bool isReal = false;
        while ((isdigit(ch) || ch == 'e' || ch == 'E' || ch == '.' || ch == '+' || ch == '-') && i < 19) {
            if (ch == 'e' || ch == 'E' || ch == '.') {
                isReal = true;
            }
            TOKEN[i] = ch;
            i++;
            ch = fgetc(fp);
        }
        TOKEN[i] = '\0';
        fseek(fp, -1, 1);
        if (isReal) {
            double x = 0;
            if (realDFA.Query((char*)TOKEN, x)) {
                out(REAL, x); // To do: identify int and float number
            } else {
                std::string err = "Can't identify the real number ";
                report_error((char*)((err + TOKEN).c_str()));
                return false;
            }
        } else {
            int x = 0;
            if (intDFA.Query((char*)TOKEN, x)) {
                out(INT, x);
            } else {
                std::string err = "Can't identify the real number ";
                report_error((char*)((err + TOKEN).c_str()));
                return false;
            }
        }
        
    } else {
        switch (ch) {
        case '<':
            ch = fgetc(fp);
            if (ch == '=') {
                out(LE, (char*)" ");
            } else if (ch == '£¾') {
                out(NE, (char*)" ");
            } else {
                fseek(fp, -1, 1);
                out(LT, (char*)" ");
            }
            break;
        case '=':
            out(EQ, (char*)" ");
            break;
        case '>':
            ch = fgetc(fp);
            if (ch == '=') {
                out(GE, (char*)" ");
            } else {
                fseek(fp, -1, 1);
                out(GT, (char*)" ");
            }
            break;
        case ':':
            ch = fgetc(fp);
            if (ch == '=') {
                out(IS, (char*)" ");
            } else {
                report_error((char*)"expect = after token :");
                return false;
            }
            break;
        case '+':
            out(PL, (char*)" ");
            break;
        case '-':
            out(MI, (char*)" ");
            break;
        case '*':
            out(MU, (char*)" ");
            break;
        case '/':
            out(DI, (char*)" ");
            break;
        default:
            if (ch == EOF || ch == ' ' || ch == '\n' || ch == '\r' || ch == '\0' || ch == '\t') {
                return true;
            }
            report_error((char*)"unknown identifier");
            return false;
            break;
        }
    }
    return true;
}
