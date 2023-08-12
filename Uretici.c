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

    sem_t *sem_for_tuketici = sem_open("/sem_for_tuketici", O_CREAT, 0644, 0);
    sem_t *sem_for_uretici = sem_open("/sem_for_uretici", O_CREAT, 0644, 1);  // Producer starts first

    while (1) {
        sem_wait(sem_for_uretici);  // Wait for the signal from Tuketici
        for (int i = 0; i < 10; i++) {
            pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
            pthread_join(threads[i], NULL);
        }
        sayac = 0;
        sem_post(sem_for_tuketici);  // Signal to Tuketici to start consuming
    }

    sem_close(sem_for_tuketici);
    sem_unlink("/sem_for_tuketici");

    sem_close(sem_for_uretici);
    sem_unlink("/sem_for_uretici");

    return 0;
}
