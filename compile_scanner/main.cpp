﻿#include <cstdio>
#include <iostream>

#include "scanner.h"

char filename[10] = "test.txt";

int main() {
	//printf("Please input source file name:\n");
	//scanf_s("%s", filename, 9);
	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, filename, "r");
	// freopen("result.txt", "w", stdout);
	while (err != 0) {
		printf("Fail to Open the file, please check your file name.\n");
		scanf_s("%s", filename);
		err = fopen_s(&fp, filename, "r");
	}
	set_current_row(0);
	set_outMode(0);
	while (!feof(fp)) {
		if (!main_scanner(fp)) {
			break;
		}
	}
	fclose(fp);
	return 0;
}
