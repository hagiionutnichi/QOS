#include "string.h"
#include <stdint.h>

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void hex_to_ascii(int n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    int32_t tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

/* K&R */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* K&R */
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

/* K&R 
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

char* strtok(char* s, char* delm)
{
    static int currIndex = 0;
    static char* currStr;
    if(s) {
        currIndex = 0;
        currStr = (char *)GlobalAllocator.RequestPage();
        strcpy(s, currStr, 0);
    }
    if(!currStr || !delm || currStr[currIndex] == '\0') {
    return 0;
    }

    char *W = (char *)GlobalAllocator.RequestPage();
    int i = currIndex, k = 0, j = 0;

    while (currStr[i] != '\0'){
        j = 0;
        while (delm[j] != '\0'){
            if (currStr[i] != delm[j])
                W[k] = currStr[i];
            else goto It;
            j++;
        }

        i++;
        k++;
    }
It:
    W[i] = 0;
    currIndex = i+1;
    //Iterator = ++ptr;
    return W;
}


void strcpy(char* src, char* dest, int offset) {
    int i = 0;
    while (src[i + offset] != '\0') {
        dest[i] = src[i + offset];
        i++;
    }

    //copy the \0
    dest[i] = '\0';
}


bool begins(char* s, char* with) {
    int i = 0;
    
    while(with[i] != '\0') {
        if(s[i] != with[i])
            return false;
        i++;
    }
    return true;
}