﻿#include "analyzer.h"

char filename[10] = "test.txt";

int main()
{
	FILE* fp = nullptr, * resultFlow = nullptr;
	// printf("Please input source file name:\n");
	// scanf_s("%s", filename, 9);
	errno_t err = fopen_s(&fp, filename, "r");
	while (err != 0) {
		printf("Fail to Open the file, please check your file name.\n");
		scanf_s("%s", filename);
		err = fopen_s(&fp, filename, "r");
	}
    start_analyze(fp);
	fclose(fp);
	return 0;
}