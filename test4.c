#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
    int err = execlp("exit", "exit", NULL);
    if (err == -1) {
        printf("Failed\n");
    }
    return 0;
}
