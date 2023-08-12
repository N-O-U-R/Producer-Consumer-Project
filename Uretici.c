#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

int sayac = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_fonksiyonu(void *arg) {
    pthread_mutex_lock(&mutex);
    sayac++;
    printf("Üretici thread %d: %d yazıldı.\n", sayac, sayac);
    pthread_mutex_unlock(&mutex);
    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[10];

    sem_t *sem_uretici = sem_open("/sem_uretici", O_CREAT, 0644, 0);

    while (1) {
        for (int i = 0; i < 10; i++) {
            pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
            pthread_join(threads[i], NULL);
            sem_post(sem_uretici);
        }
        sayac = 0;
    }

    sem_close(sem_uretici);
    sem_unlink("/sem_uretici");
    return 0;
}
