#pragma once
#include <stdint.h>
#include "Memory/PageFrameAllocator.h"

void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], char s2[]);
void hex_to_ascii(int n, char str[]);
char* strtok(char* s, char* delm);
bool begins(char* s, char* with);
void strcpy(char* src, char* dest, int offset);

