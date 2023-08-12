#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int sayac = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int fd[2];  // for data
int sync_pipe[2];  // for synchronization

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

    if (pipe(fd) == -1 || pipe(sync_pipe) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) {  // Yavru süreç
        close(fd[1]);
        close(sync_pipe[0]);

        char signal;
        while (1) {
            for (int i = 0; i < 10; i++) {
                pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
                pthread_join(threads[i], NULL);
            }
            
            write(sync_pipe[1], "P", 1);  // Signal to Tuketici that production is done
            sayac = 0;
            read(sync_pipe[1], &signal, 1);  // Wait for signal from Tuketici
        }
    }

    return 0;
}
