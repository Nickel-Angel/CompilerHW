#include "lookup.h"

/* 建立保留字表 */
constexpr auto MAX_KEY_NUMBER = 6;                          /*关键字的数量*/
constexpr char* KeyWordTable[MAX_KEY_NUMBER] = {
    (char*)"begin", (char*)"end", (char*)"const",
    (char*)"var", (char*)"if", (char*)"else"
};
/* 查保留字表，判断是否为关键字 */
int lookup(char* token) {
    int n = 0;
    while (n < MAX_KEY_NUMBER) { /*strcmp比较两串是否相同，若相同返回0*/
        if (!strcmp(KeyWordTable[n], token)) { /*比较token所指向的关键字和保留字表中哪个关键字相符*/
            return n + 1; /*根据单词分类码表I，设置正确的关键字类别码，并返回此类别码的值*/
        }
        ++n;
    }
    return 0; /*单词不是关键字，而是标识符*/
}
