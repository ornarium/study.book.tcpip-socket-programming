#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
        int n, fd[2];
        int pid;
        int i = 0;
        int j = 0;

        //파이프를 생성
        if(pipe(fd) < 0) {
                perror("pipe error : ");
                exit(0);
        }
        printf("my pid is %d \n", getpid());

        //fork함수를 이용해서 자식 프로세스를 생성한다
        //fork가 실패할 경우 프로그램을 종료시킨다
        if((pid = fork()) < 0) {
                perror("fork error : ");
                exit(0);
        }

        //자식프로세스는 읽기 전용 파이프로부터 데이터를 읽어들인다
        else if(pid == 0) {
                close(fd[1]);
                while(1) {
                        read(fd[0], (void *)&j, sizeof(int));
                        printf("read data %d\n", j);
                }
        }
        
        //부모 프로세스는 쓰기 전용 파이프에 1초 간격으로 데이터 씀
        else {
                close(fd[0]);
                while(1) {
                        ++i;
                        write(fd[1], (void *)&i, sizeof(int));
                        sleep(1);
                }
        }
        
}
