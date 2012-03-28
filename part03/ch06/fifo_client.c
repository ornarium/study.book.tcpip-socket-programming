#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

//서버의 선언과는 반대
#define FIFO_READ "/tmp/fifo_s"
#define FIFO_WRITE "/tmp/fifo_c"

int main() {
        int fp_w;
        int fp_r;
        int i = 1;

        if((fp_r = open(FIFO_READ, O_RDWR)) < 0) {
                perror("open error : ");
                exit(0);
        }

        if((fp_w = open(FIFO_WRITE, O_RDWR)) < 0) {
                perror("open error : ");
                exit(0);
        }

        while(1) {
                sleep(1);
                write(fp_w, (void *) &i, sizeof(int));
                read(fp_r, (void *)&i, sizeof(int));
                printf("서버로부터 데이터 읽음 %d\n", i);
        }
        exit(1);
}
