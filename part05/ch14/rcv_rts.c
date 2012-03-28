#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// 시그널 핸들러
void sighandler(int signo, siginfo_t * si) {
        //시그널이 리얼타임 시그럴인지, 일반 유닉스 시그널인지 확인
        if(si->si_code == SI_QUEUE) {
                printf("User RTS signal %d\n", si->si_pid);
                printf("Sig Number %d\n", si->si_signo);
                printf("User Data is %d\n", si->si_value.sival_int);
                //키보드 입력을 기다리도록 해서, RT signal이 대기큐에서 기다리는지
                //기다리지 않는지 시험용
                getchar();
                
        } else {
                printf("Get none realtime signal %d \n", signo);
        }
}

int main() {
        struct sigaction sigact;

        printf("My pid %d \n", getpid());

        //리얼타임 시그널을 등록
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags = SA_SIGINFO;
        sigact.sa_restorer = NULL;
        sigact.sa_sigaction = sighandler;

        if(sigaction(SIGRTMIN, &sigact, 0) == 1) {
                printf("signal error\n");
                exit(0);
        }
        while(1) {
                sleep(1);
        }
        
}
