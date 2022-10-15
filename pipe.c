#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int i;
    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }

    int pid1 = fork();
    if (pid1 < 0) {
        return 2;
    }

    if (pid1 == 0) {
        // Chlid process 1 (ls)
        dup2(fd[1], 1);
        i = fd[1];
        printf("fd[1]: %d\n", i);
        close(fd[0]);
        close(fd[1]);
        execlp("ls", "ls", "-l", NULL);
    }
    printf("fd[0]: %d\n", fd[0]);
    printf("fd[0]: %d\n", fd[1]);

    int pid2 = fork();
    if (pid2 < 0) {
        return 3;
    }

    if (pid2 == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "shell", NULL);
    }
    close(fd[0]); // for main process
    close(fd[1]); // for main process

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
