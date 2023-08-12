#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

void *thread_fonksiyonu(void *arg) {
    int okunan_deger = *((int*) arg);
    printf("Tüketici thread: %d okundu.\n", okunan_deger);
    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[10];

    sem_t *sem_uretici = sem_open("/sem_uretici", O_CREAT, 0644, 0);

    int counter = 0;
    while (1) {
        for (int i = 0; i < 10; i++) {
            sem_wait(sem_uretici);
            counter++;
            pthread_create(&threads[i], NULL, thread_fonksiyonu, &counter);
            pthread_join(threads[i], NULL);
        }
        counter = 0;
    }

    sem_close(sem_uretici);
    sem_unlink("/sem_uretici");
    return 0;
}
