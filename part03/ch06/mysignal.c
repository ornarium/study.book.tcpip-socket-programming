#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo);

int main() {
        int i = 0;
        signal(SIGINT, (void *)sig_handler); //Ctrl+C 무시

        while(1) {
                printf("%d\n", i);
                ++i;
                sleep(1);
        }

        return 1;
}

void sig_handler(int signo) {
        printf("SIGINT 발생\n");
}
