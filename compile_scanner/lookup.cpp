#include "lookup.h"

/* ���������ֱ� */
constexpr auto MAX_KEY_NUMBER = 6;                          /*�ؼ��ֵ�����*/
constexpr char* KeyWordTable[MAX_KEY_NUMBER] = {
    (char*)"begin", (char*)"end", (char*)"const",
    (char*)"var", (char*)"if", (char*)"else"
};
/* �鱣���ֱ��ж��Ƿ�Ϊ�ؼ��� */
int lookup(char* token) {
    int n = 0;
    while (n < MAX_KEY_NUMBER) { /*strcmp�Ƚ������Ƿ���ͬ������ͬ����0*/
        if (!strcmp(KeyWordTable[n], token)) { /*�Ƚ�token��ָ��Ĺؼ��ֺͱ����ֱ����ĸ��ؼ������*/
            return n + 1; /*���ݵ��ʷ������I��������ȷ�Ĺؼ�������룬�����ش�������ֵ*/
        }
        ++n;
    }
    return 0; /*���ʲ��ǹؼ��֣����Ǳ�ʶ��*/
}
