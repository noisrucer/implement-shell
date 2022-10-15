#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isAllSpaces(char* str) {
    if (strlen(str) == 0) return 0;
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        if (str[i] != ' ') return 0;
    }
    return 1;
}

int isEmpty(char* str) {
    return strlen(str) == 0;
}

char* trim(char* str) {
    if (isEmpty(str) || isAllSpaces(str)) return str;

    int len = strlen(str);
    int i = 0, j = len - 1;

    while (str[i] == ' ') i++;
    while (str[j] == ' ') j--;

    char* temp = malloc((j - i + 2) * sizeof(char));
    temp[j - i + 1] = '\0';
    strncpy(temp, str + i, j - i + 1);

    return temp;
}

char* substrFromZero(char* str, int len) { // [i, j] both inclusive
    char* temp = malloc((len + 1) * sizeof(char));
    strncpy(temp, str, len);
    temp[len] = '\0';
    return temp;
}

int findTokenNum(char* input) {
    int token_num = 1; // total # tokens = 1 + spaces
    int i = 0;
    while (input[i] != '\0') {
        if (input[i] == ' ')
            token_num++;
        i++;
    }

    return token_num;
}

char* getFirstCommand(char* single_cmd) {
    char* breakPoint = strstr(single_cmd, " ");
    char* start = single_cmd;
    int len;
    char* firstCommand;
    if (breakPoint == NULL) { // single word
        len = strlen(single_cmd);
    } else {
        len = breakPoint - start;
    }
    firstCommand = substrFromZero(single_cmd, len);
    return firstCommand;
}

// Return simply wheter use timeX or not
int usetx_func (char* single_cmd) {
    // char* single_cmd: "timeX ls -l -a" or "ls -l -a"
    char* firstCommand = getFirstCommand(single_cmd);
    return (strcmp(firstCommand, "timeX") == 0);
}

// If use TimeX, return command after TimeX
// Guarantee that it's a proper timeX-including command with multiple tokens
char* removeTimeX(char* single_cmd) {
    // "timeX ls -l" -> "ls -l"
    return single_cmd + 6;
}

char** breakIntoTokens (char* cmd, int numToken, char** command) {
    // char* cmd: "ls -l -a"
    // int numToken: 3
    // char** command: call by reference

    char** tokens = malloc((numToken + 1) * sizeof(char*));

    char* breakPoint = strstr(cmd, " ");
    char* start = cmd;
    int len;
    int i = 0;

    while (breakPoint != NULL) {
        len = breakPoint - start;
        tokens[i] = malloc((len + 1) * sizeof(char));
        strncpy(tokens[i], start, len);
        tokens[i][len] = '\0';
        start += (len + 1);
        printf("tokens[%d]: %s\n", i, tokens[i]);
        i++;
        breakPoint = strstr(start, " ");
    }

    int totalLen = strlen(cmd);
    int lastCmdLen = (cmd + totalLen) - start;
    char* lastCmd = substrFromZero(start, lastCmdLen);

    tokens[i] = malloc((lastCmdLen + 1) * sizeof(char));
    strcpy(tokens[i], lastCmd);
    tokens[i][lastCmdLen] = '\0';
    printf("tokens[%d]: %s\n", i, tokens[i]);

    tokens[numToken] = '\0';
    *command = getFirstCommand(cmd);

    return tokens;
}

char** parse_single_cmd (char* single_cmd, int* useTimeX, int* invalidTimeX, char** command) {
    /*
    Call by reference:
        int* useTimeX: 0 or 1
        int* invalidTimeX: 0 or 1
        char** command
    Returns:
        char** argVec
    */
    int tokenNum = findTokenNum(single_cmd);
    int removedTimeXTokenNum = tokenNum;
    int usetx = usetx_func(single_cmd);

    // If using timeX
    if (usetx) {
        *useTimeX = 1; // set timeX flag to 1
        if (tokenNum == 1) { // standalone timeX error
            *invalidTimeX = 1;
            return NULL;
        }

        // Parse single_cmd by removing timeX
        single_cmd = removeTimeX(single_cmd); // ex) "ls -l"
        removedTimeXTokenNum--; // removedTimeXTokenNum = 2
    }
    printf("removedTimeXTokenNum: %d\n", removedTimeXTokenNum);
    printf("processed single_cmd: %s\n", single_cmd);

    char** tokens = breakIntoTokens(single_cmd, removedTimeXTokenNum, &*command);
    printf("%s\n", *tokens);
    printf("%s\n", *(tokens + 1));
    printf("%s\n", *(tokens + 2));
    printf("command: %s\n", *command);

    return tokens;
}

char** parse_input (char* input, int* total_num_Cmds, int* errFlag) {
    // errFlag = 0: invalid usage of | (as a first or last character of the input)
    // errFlag = 1: two consecutive || 
    // If both errFlag 0 and errFlag 1, report errFlag 0
    
    // trim input (remove spaces front and back)
    input = trim(input);
    char* start = input;
    char* breakPoint = strstr(input, "|");
    char** output = malloc(1 * sizeof(char*));
    int numCmds = 0;
    int i = 0;
    int len;
    int invalidFlag = 0;
    int invalidConsecutive = 0;

    // Invalid Usage: | at the front or back
    if (input[0] == '|' || input[(int)(strlen(input) - 1)] == '|') {
        printf("3230shell: invalid usage of |\n");
        *errFlag = 0;
        return NULL;
    }

    while (breakPoint != NULL) {
        len = breakPoint - start;
        char* temp = substrFromZero(start, len);
        if (isEmpty(temp) || isAllSpaces(temp)) {
            printf("3230shell: should not have two consecutive | without in-between command\n");
            *errFlag = 1;
            return NULL;
        }

        char* trimmed_cmd = trim(temp);
        int trimmed_len = (int)strlen(trimmed_cmd);

        // Store command into output
        output[i] = malloc((trimmed_len + 1) * sizeof(char));
        strcpy(output[i], trimmed_cmd);
        output[i][trimmed_len] = '\0'; // mark the end of string
        
        start += (len + 1); // jump to next start of the command

        numCmds++;
        // Reallocate one more memory block for each loop
        output = realloc(output, (numCmds + 1) * sizeof(char *));
        
        breakPoint = strstr(start, "|");
        /** printf("%s,%d\n", output[i], (int)strlen(output[i])); */
        i++;
    }

    // Handle the last command
    int totalLen = strlen(input);
    int lastCmdLen = (input + totalLen) - start;
    char* last_temp = substrFromZero(start, lastCmdLen);
    char* trimmed_last_cmd = trim(last_temp);
    int trimmed_last_cmd_len = (int)strlen(trimmed_last_cmd);
    output[i] = malloc((trimmed_last_cmd_len + 1) * sizeof(char));
    strcpy(output[i], trimmed_last_cmd);
    output[i][trimmed_last_cmd_len] = '\0';
    /** printf("%s,%d\n", output[i], (int)strlen(output[i])); */
    numCmds++;

    // Also record the total number of commands
    *total_num_Cmds = numCmds;

    return output;
}

int main() {
    char* input = "ls -l -f | grep rtt | abcde -l | asfsddsf -a      ";
    int numCmds;
    int errFlag = -1; // -1: no error
    char** parsed = parse_input(input, &numCmds, &errFlag);
    printf("# cmds: %d\n", numCmds);
    printf("error flag: %d\n", errFlag);

    /** char* str = "timeX ls -l -a"; */
    /** char* temp = useTimeX(str) */
    /** char *single_cmd = "timeX ls -l -a"; */
    /** int useTimeX = 0; */
    /** int invalidTimeX = 0; */
    /** char* command; */
    /** char** argVec = parse_single_cmd(single_cmd, &useTimeX, &invalidTimeX, &command); */

/** char** parse_single_cmd (char* single_cmd, int* useTimeX, int* invalidTimeX, char** command) { */


    return 0;
}
