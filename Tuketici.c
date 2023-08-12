#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int fd[2];  // for data
int sync_pipe[2];  // for synchronization

void *thread_fonksiyonu(void *arg) {
    int okunan_deger;

    read(fd[0], &okunan_deger, sizeof(okunan_deger));
    printf("Tüketici thread: %d okundu.\n", okunan_deger);

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
        close(sync_pipe[1]);

        char signal;
        while (1) {
            read(sync_pipe[0], &signal, 1);  // Wait for signal from Uretici

            for (int i = 0; i < 10; i++) {
                pthread_create(&threads[i], NULL, thread_fonksiyonu, NULL);
                pthread_join(threads[i], NULL);
            }

            write(sync_pipe[0], "C", 1);  // Signal to Uretici that consumption is done
        }
    }

    return 0;
}
