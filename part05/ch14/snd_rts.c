#include <signal.h>
#include <stdio.h>

//argv[1]은 리얼타임 시그널을 받을 프로세스의 ID
//argv[2]는 보내고자 하는 값이다
int main(int argc, char ** argv) {
        union sigval sv;

        sv.sival_int = atoi(argv[2]);
        sigqueue(atoi(argv[1]), SIGRTMIN, sv);
}
