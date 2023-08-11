#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int sayac = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int fd[2];  // pipe için

void *thread_fonksiyonu(void *arg) {
    int id = *((int *)arg);

    pthread_mutex_lock(&mutex);
    
    write(fd[1], &sayac, sizeof(sayac));
    printf("Üretici thread %d: %d yazıldı.\n", id, sayac);
    sayac++;
    pthread_mutex_unlock(&mutex);
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
        close(fd[1]);
        while (1) {
            int okunan_deger;
            for (int i = 0; i < 10; i++) {
                read(fd[0], &okunan_deger, sizeof(okunan_deger));
                printf("Yavru Üretici: %d okundu.\n", okunan_deger);
            }
        }
    } else if (pid > 0) {  // Ana süreç
        close(fd[0]);
        while (1) {
            for (int i = 1; i <= 10; i++) {
                thread_ids[i] = i;
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
