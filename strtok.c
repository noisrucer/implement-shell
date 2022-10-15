#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char* str = "abcde fgas agsadg";
    /** char temp[] = malloc((strlen(str) + 1) * sizeof(char)); */
    char* token = strtok(str, " ");

    return 0;
}

