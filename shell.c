// (char *)0
// why cd doesn't work? should I use chdir

/*
CUSTOM ERROR NUM
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <setjmp.h>

#include "COMP3230_signal.h"

#define MAX_INPUT_CHAR 1025 // including \0 
#define MAX_INPUT_STRING 30

static int fg_proc_num = 0;
static int SIGUSR1_READY = 0; // For SIGUSR1 signal to mark the child process to be ready 

void sigint_3230shell(int sig) {
    if (fg_proc_num == 0) {
        printf("\nCOMP3230shell ## ");
        fflush(stdout);    
    }
}

void sigusr1(int sig) {
    SIGUSR1_READY = 1;
}

char* handle_timeX(struct rusage usage, pid_t child_pid, char* cmd) {
    // Extract only command name
    char* token = strtok(cmd, "/");
    char* last_cmd = token;

    while(token != NULL) {
        strcpy(last_cmd, token);
        token = strtok(NULL, "/");
    }
    
    // Handle user/sys time
    int user_sec = usage.ru_utime.tv_sec;
    int user_usec = usage.ru_utime.tv_usec;
    int sys_sec = usage.ru_stime.tv_sec;
    int sys_usec = usage.ru_stime.tv_usec;
    float utime = (float)user_sec + (0.000001) * (float)user_usec;
    float stime = (float)sys_sec + (0.000001) * (float)sys_usec;

    // Report result
    printf("(PID)%d  (CMD)%s    (user)%.3f s  (sys)%.3f s\n", child_pid, last_cmd, utime, stime);
}

int main(int argc, char* argv[]) {
    char comInput[MAX_INPUT_CHAR];

    // Register SIGINT handler
    signal(SIGINT, &sigint_3230shell);
    signal(SIGUSR1, &sigusr1);

    while (1) {
        SIGUSR1_READY = 0;
        fg_proc_num = 0;

        memset(comInput, 0x00, sizeof(char) * (MAX_INPUT_CHAR - 1));

        // Read command line input
        printf("COMP3230shell ## ");
        fgets(comInput, MAX_INPUT_CHAR, stdin);
        comInput[strlen(comInput) - 1] = 0x00; // set to NULL

        // Handle exit command
        if (strcmp(comInput, "exit") == 0) { // If input exit, then terminate the shell
            printf("3230shell: Terminated\n");
            return 0;
        }

        // Parse command line command & arguments
        // [1] Find the number of tokens for dynamic memory allocation
        int token_num = 1; // total # tokens = 1 + spaces
        int i = 0;
        while (comInput[i] != '\0') {
            if (comInput[i] == ' ')
                token_num++;
            i++;
        }

        // [2] Initialize command and argVec for execvp later
        char* command;
        int useTimeX = 0;
        char** argVec = (char**)malloc((token_num + 1) * sizeof(char *));

        char* token;
        char* delim = " ";
        token = strtok(comInput, delim);
        i = 0;

        while (token != NULL) {
            if (i == 0) {
                command = (char*)malloc(strlen(token) * sizeof(char));
                if (strcmp(token, "timeX") == 0) {
                    useTimeX = 1;
                    token_num--;
                    token = strtok(NULL, delim);
                    continue;
                }

                strcpy(command, token);
            }
            argVec[i] = (char*)malloc(strlen(token) * sizeof(char));
            strcpy(argVec[i], token); // copy token to argVec
                
            token = strtok(NULL, delim);
            i++;
        }
        argVec[token_num] = (char *) 0;

        /** printf("command: %s\n", command); */
        /** for (i = 0; i < token_num; i++) { */
        /**     printf("arg[%d]: %s\n", i, argVec[i]); */
        /** } */


        if (useTimeX && token_num == 0) {
            printf("3230shell: \"timeX\" cannot be a standalone command\n");
            continue;
        }

        if (!useTimeX && strcmp(command, "exit") == 0 && token_num > 1) {
            printf("3230shell: \"exit\" with other arguments!!!\n");
            continue;
        }

        pid_t pid = fork();
        
        if (pid == 0) { // child process
            // Execute only if upon the parent's SIGUSR1 signal
            while (1) {
                if (SIGUSR1_READY == 1) break;
                usleep(10);
            }
            
            int err = execvp(command, argVec);
            if (err == -1) {
                // execution failed
                printf("Execution failed..\n");
                return 1;
            }
        } else { // parent process
            kill(pid, SIGUSR1);
            fg_proc_num++;

            int status;
            struct rusage usage;
            pid_t child_pid = wait4((pid_t)-1, &status, 0, &usage);

            if (WIFEXITED(status)) {
                if (useTimeX) {
                    handle_timeX(usage, child_pid, command);
                }
                int statusCode = WEXITSTATUS(status);

                if (statusCode == 0) {
                    printf("Success!\n");
                } else {
                    printf("Faillure!\n");
                }
            }
            else if (WIFSIGNALED(status)) {
                int sigNum = WTERMSIG(status);
                char* term_sig_desc = signal_des[sigNum];
                printf("%s\n", term_sig_desc);
            }

        }
        
    }

    
}
