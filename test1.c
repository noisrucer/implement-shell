#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]) {
    char* comInput = "ls -l -h";
    comInput[strlen(comInput) - 1] = 0x00;
    /** int token_num = 1; */
    /** int i = 0; */
    /** while (comInput[i] != '\0') { */
    /**     if (comInput[i] == ' ') */
    /**         token_num++; */
    /**     i++; */
    /** } */

    /** printf("Number of tokens: %d\n", token_num); */

    /** char** argVec = malloc(token_num * sizeof(char *)); */

    char* token;
    char* delim = " ";
    int cnt = 0;
    token = strtok(comInput, delim);
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, delim);
        cnt++;
    }

    printf("number of actual tokens: %d\n", cnt);

    

}
