#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//우리가 접근을 제어하고자 하는 전역 변수
int ncount;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * do_loop(void * data) {
        int i;
        int id = *((int *) data);
        int mydata;

        for(i = 0; i < 10; ++i) {
                //ncount 는 공유되는 자원이기 때문에
                //뮤텍스를 이용해서 보호
                pthread_mutex_lock(&mutex);

                //-- 임계 영역 진입
                printf("thread %d : %d\n", id, ncount);
                mydata = ncount;
                ++mydata;
                usleep(1000);
                ncount = mydata;
                //-- 임계영역 끝
                pthread_mutex_unlock(&mutex);
                usleep(10);
        }
        return 0;
}

int main(){
        int thr_id;
        pthread_t p_thread[3];
        int status;
        int i;
        ncount = 0;

        //스레드를 생성
        for(i = 0; i < 3; ++i) {
                thr_id = pthread_create(&p_thread[i], NULL, do_loop, (void *) &i);
        }

        for(i = 0; i < 3; ++i) {
                pthread_join(p_thread[i], (void *)&status);
        }
        //모든 작업이 끝났다면 뮤텍스 객체를 소멸시킨다.
        status = pthread_mutex_destroy(&mutex);
        printf("programing is end\n");
        return 0;
}
        
