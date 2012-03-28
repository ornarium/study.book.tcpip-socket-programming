#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
        int pid;
        int i = 0;
        printf("process begin %d\n", getpid());
        pid = fork();
        if(pid > 0) {
                printf("parent process pid(%d) \n", getpid());
                exit(0);
        } else if(pid == 0) {
                sleep(1);
                printf("child process pid(%d) : %d \n", getpid(), getppid());
                close(0);
                close(1);
                setsid();
                printf("iam daemon.\n");
                i = 1000;
                while(1) {
                        printf("child: %d \n", i);
                        ++i;
                        sleep(2);
                }
                
        }
        
        return 1;
}
