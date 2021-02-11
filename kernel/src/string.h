#pragma once
#include <stdint.h>
#include "Memory/PageFrameAllocator.h"

void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], char s2[]);
int strncmp(const char s1[], const char s2[], unsigned int n);
void hex_to_ascii(int n, char str[]);
char* strtok(char* s, char* delm);
bool begins(char* s, char* with);
void strcpy(char* src, char* dest, int offset);
int strargcmp(char* command, size_t argumentIndex, const char* cmp, unsigned int cmpLength);
uint64_t strargval(char* command, size_t argumentIndex);

uint64_t myAtoi(char* str);
uint64_t myAtoiChar(char* str, const char c);