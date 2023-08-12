#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

int fd[2];  // pipe için

void *thread_fonksiyonu(void *arg) {
    int okunan_deger;

    sem_t *sem_uretici = sem_open("/sem_uretici", O_CREAT, 0644, 0);
    sem_t *sem_tuketici = sem_open("/sem_tuketici", O_CREAT, 0644, 10);
    sem_wait(sem_uretici);  // Wait for Uretici signal

    read(fd[0], &okunan_deger, sizeof(okunan_deger));
    printf("Tüketici thread: %d okundu.\n", okunan_deger);
    sem_post(sem_tuketici);  // Signal to Uretici that a number is consumed

    sem_close(sem_uretici);
    sem_close(sem_tuketici);

    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[10];

    if(pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    while(1) {
        for(int i = 0; i < 10; i++) {
            pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
            pthread_join(threads[i], NULL);
        }
    }
    return 0;
}
