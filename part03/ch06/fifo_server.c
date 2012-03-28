#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

//클라이언트의 선언과는 반대
#define FIFO_READ "/tmp/fifo_c"
#define FIFO_WRITE "/tmp/fifo_s"

int main() {
        int fp_w;
        int fp_r;
        int i = 0;

        //파일 생성시 권한에 대한 마스크 설정
        umask(0000);

        //mkfifo를 이용해서 FIFO파일 생성
        //FIFO_READ는 클라이언트로부터 데이터를 읽기 위해서 사용
        if(mkfifo(FIFO_READ, 0666) != 0) {
                perror("mkfifo failure");
        }

        //FIFO_WRITE는 클라이언트로 데이터를 쓰기 위해서 사용
        if(mkfifo(FIFO_WRITE, 0666) != 0) {
                perror("mkfifo failure");
        }

        if((fp_r = open(FIFO_READ, O_RDWR)) < 0) {
                perror("open error : ");
                exit(0);
        }

        if((fp_w = open(FIFO_WRITE, O_RDWR)) < 0) {
                perror("open error : ");
                exit(0);
        }

        //read를 통해 int형 숫자를 읽어 2를 곱해서 클라이언트
        //에게 되돌려줌
        while(read(fp_r, (void *)&i, sizeof(int)) > 0) {
                printf("클라인어트로 부터 데이터 읽음 %d\n", i);
                i = i * 2;
                write(fp_w, (void *)&i, sizeof(int));
        }
        exit(1);
}
