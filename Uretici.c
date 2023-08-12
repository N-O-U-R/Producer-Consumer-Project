#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

int sayac = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int fd[2];  // for data

void *thread_fonksiyonu(void *arg) {
    pthread_mutex_lock(&mutex);
    sayac++;
    write(fd[1], &sayac, sizeof(sayac));
    printf("Üretici thread %d: %d yazıldı.\n", sayac, sayac);
    pthread_mutex_unlock(&mutex);
    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[10];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    sem_t *sem_uretici = sem_open("/sem_uretici", O_CREAT, 0644, 0);  // Uretici semaphore
    sem_t *sem_tuketici = sem_open("/sem_tuketici", O_CREAT, 0644, 10);  // Tuketici semaphore

    while (1) {
        for (int i = 0; i < 10; i++) {
            pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
            pthread_join(threads[i], NULL);
            sem_wait(sem_tuketici);  // Decrement the Tuketici semaphore
        }

        for (int i = 0; i < 10; i++) {
            sem_post(sem_uretici);  // Signal to Tuketici that a number is ready
        }
        sayac = 0;
    }

    sem_close(sem_uretici);
    sem_close(sem_tuketici);
    return 0;
}
