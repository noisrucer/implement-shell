#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

char* handle_timeX (char* arr) {

}

int main (int argc, char* argv[]) {
    int fd[2]; // 1 for write, 0 for read
    int saved_stdout;
    if (pipe(fd) == -1) {
        printf("Pipe failed!\n");
        return 1;
    }
    pid_t pid = fork();

    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        dup2(fd[1], 2);
        close(fd[1]);
        int err = execlp("time", "time", "ps", "f", NULL);
    } else {
        /** dup2(saved_stdout, 1); */

        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Entered\n");
            close(fd[1]);
            char foo[4096];
            int nbytes = read(fd[0], foo, sizeof(foo));
            printf("%s\n", foo);
            int statusCode = WEXITSTATUS(status);

            if (statusCode == 0) {
                printf("Success!\n");
            } else {
                printf("Faillure!\n");
            }
        }
    }

    return 0;
}
