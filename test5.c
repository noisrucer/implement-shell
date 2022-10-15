#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

static int i = 0;

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        i = 5;
        execlp("ls", "ls", NULL);
    } else {
        int status;
        wait(&status);
        i = 5;
        printf("i: %d\n", i);

        if (WIFSIGNALED(status)) {
            int sigNum = WTERMSIG(status);
            printf("sigNum: %d\n", sigNum);
        }
    }
}
