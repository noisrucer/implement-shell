#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/time.h>

char* handle_timeX (char* arr) {
    
}

int main (int argc, char* argv[]) {
    pid_t pid = fork();
    struct rusage usage;
    int fd[2];
    pipe(fd);

    if (pid == 0) {
        close(0);
        write()
        int err = execlp("ps", "f", NULL);
    } else {

        int status;
        pid_t child_pid = wait4((pid_t)-1, &status, 0, &usage);

        int user_sec = usage.ru_utime.tv_sec;
        int user_usec = usage.ru_utime.tv_usec;
        int sys_sec = usage.ru_stime.tv_sec;
        int sys_usec = usage.ru_stime.tv_usec;
        float utime = (float)user_sec + (0.000001) * (float)user_usec;
        float stime = (float)sys_sec + (0.000001) * (float)sys_usec;
        
        printf("(PID)%d\n", child_pid);
        printf("(user)%.3f s\n", utime);
        printf("(sys)%.3f s\n", stime);

        if (WIFEXITED(status)) {
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
