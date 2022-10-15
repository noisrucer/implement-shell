#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig) {
    printf("CTRL C detected\n");
    signal(SIGINT, SIG_DFL);
}

int main() {
    /** signal(SIGINT, &handler); */
    int sec = 8;
    int cnt = 0;
    while (cnt < sec) {
        /** printf("Hello~!\n"); */
        sleep(1);
        cnt++;
    }
}

