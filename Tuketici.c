#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int fd[2];  // pipe için

void *thread_fonksiyonu(void *arg) {
    int okunan_deger;
    int id = *((int *)arg);

    read(fd[0], &okunan_deger, sizeof(okunan_deger));
    printf("Tüketici thread %d: %d okundu.\n", id, okunan_deger);

    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_ids[10];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) {  // Yavru süreç
        close(fd[0]);
        int gonderilen_deger;
        while (1) {
            for (int i = 0; i < 10; i++) {
                read(STDIN_FILENO, &gonderilen_deger, sizeof(gonderilen_deger));
                write(fd[1], &gonderilen_deger, sizeof(gonderilen_deger));
            }
        }
    } else if (pid > 0) {  // Ana süreç
        close(fd[1]);
        while (1) {
            for (int i = 0; i < 10; i++) {
                thread_ids[i] = i + 1;
                pthread_create(&threads[i], NULL, thread_fonksiyonu, &thread_ids[i]);
                pthread_join(threads[i], NULL);
            }
        }
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}
