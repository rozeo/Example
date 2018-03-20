#include "Base64.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <WinSock2.h>

char b64_table[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
 
void base64_encode(char *in, size_t in_len, char *output) {
	bool *out = (bool *)malloc(sizeof(bool) * in_len * 8);
	if (out == NULL) {
		return;
	}

	for (int offset = 0; offset < in_len; offset++) {
		int val = (unsigned char)in[offset];
		for (int r = 7, l = 0; r >= 0; r--, l++) {
			int a = (int)(val / pow(2, r));
			out[offset * 8 + l] = (bool)a;
			val -= a * pow(2, r);
		}
	}

	bin2b64(out, in_len * 8, output);
}

void bin2b64(bool *in, size_t in_len, char *buffer) {
	int str_offset = 0;
	for (int i = 0; i < in_len; i += 6) {
		int val = 0;
		for (int j = i; j < i + 6 && j < in_len; j++) {
			val += in[j] * pow(2, 5 - (j - i));
		}

		buffer[str_offset++] = b64_table[val];
	}
	while (str_offset % 4 != 0) buffer[str_offset++] = '=';
	buffer[str_offset] = '\0';
}

long long int file_size(char *file_name) {
	FILE *fp = fopen(file_name, "rb");
	if (fp == NULL) return 0;

	fseek(fp, 0, SEEK_END);
	long long int size = ftell(fp);
	fclose(fp);
	return size;
}
