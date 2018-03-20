#pragma once

// ������(�o�C�i����)��137%�̐��l(�����悻�̍ő咷)
#define BASE64_SIZE(x) ((int)(x * 1.37) + 1)
 
void base64_encode(char *in, size_t in_len, char *output);
void bin2b64(bool *in, size_t in_len, char *buffer);

long long int file_size(char *file_name);
