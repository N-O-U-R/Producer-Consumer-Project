#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

int fd[2];  // for data

void *thread_fonksiyonu(void *arg) {
    int okunan_deger;

    read(fd[0], &okunan_deger, sizeof(okunan_deger));
    printf("Tüketici thread: %d okundu.\n", okunan_deger);

    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[10];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    sem_t *sem = sem_open("/mysem", O_CREAT, 0644, 0);  // Open semaphore

    pid_t pid = fork();
    if (pid == 0) {  // Yavru süreç
        close(fd[1]);

        while (1) {
            sem_wait(sem);  // Wait for signal from Uretici

            for (int i = 0; i < 10; i++) {
                pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
                pthread_join(threads[i], NULL);
            }

            sem_post(sem);  // Signal to Uretici that consumption is done
        }
    }
    sem_close(sem);

    return 0;
}
