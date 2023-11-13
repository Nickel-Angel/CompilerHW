#pragma once
#ifndef _LABEL_H_
#define _LABEL_H_

constexpr auto MAX_SCANNER_LABEL_NUMBER = 23;

namespace scanner_label {
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
        LT,        // 10 <
        LE,        // 11 <=
        EQ,        // 12 =
        NE,        // 13 <>
        GT,        // 14 >
        GE,        // 15 >=
        IS,        // 16 :=
        PL,        // 17 +
        MI,        // 18 -
        MU,        // 19 *
        DI,        // 20 /
        LBU,       // 21 (
        RBU,       // 22 )
        SEM        // 23 ;
    };
}

constexpr int MAX_VN = 9;

namespace analyzer_label {
    enum VT {
        ID = 0,
        PL,
        MI,
        MU,
        DI,
        LBU,
        RBU,
        SEM,
        FIN
    };

    enum VN {
        S = 0,
        Sp,
        E,
        Ep,
        T,
        Tp,
        F,
        A,
        M
    };
}


#endif // _LABEL_H_