#include <cstdio>
#include <iostream>

#include "scanner.h"

char filename[10];

int main() {
	printf("Please input source file name:\n");
	scanf("%s", filename);
	FILE* fp = fopen(filename, "r");
	while (fp == nullptr) {
		printf("Fail to Open the file, please check your file name.\n");
		scanf("%s", filename);
		fp = fopen(filename, "r");
	}
	while (!feof(fp)) {
		if (!main_scanner(fp)) {
			break;
		}
	}
	fclose(fp);
	return 0;
}
