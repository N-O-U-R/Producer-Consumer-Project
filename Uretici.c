#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

int sayac = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int fd[2];  // pipe for communication between parent and child

void *thread_fonksiyonu(void *arg) {
    pthread_mutex_lock(&mutex);
    sayac++;
    write(fd[1], &sayac, sizeof(sayac));  // write data to child process
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

    pid_t pid = fork();
    if (pid == 0) {  // child process
        close(fd[1]);
        int okunan_deger;
        while (1) {
            read(fd[0], &okunan_deger, sizeof(okunan_deger));
            printf("Üretici child thread: %d yazıldı.\n", okunan_deger);
        }
    } else if (pid > 0) {  // parent process
        close(fd[0]);

        sem_t *sem_for_tuketici = sem_open("/sem_for_tuketici", O_CREAT, 0644, 0);
        sem_t *sem_for_uretici = sem_open("/sem_for_uretici", O_CREAT, 0644, 1);

        while (1) {
            sem_wait(sem_for_uretici);  
            for (int i = 0; i < 10; i++) {
                pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
                pthread_join(threads[i], NULL);
            }
            sayac = 0;
            sem_post(sem_for_tuketici);
        }

        sem_close(sem_for_tuketici);
        sem_unlink("/sem_for_tuketici");
        sem_close(sem_for_uretici);
        sem_unlink("/sem_for_uretici");
    }

    return 0;
}
