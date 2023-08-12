#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SEM_NAME "/semaphore_example"
sem_t *sem;

int fd[2];  // pipe için

void *thread_fonksiyonu(void *arg) {
    int okunan_deger;

    read(fd[0], &okunan_deger, sizeof(okunan_deger));
    if(okunan_deger == 0) {
        printf("No data read from pipe!\n");
        return NULL;
    }

    printf("Tüketici thread %d: %d okundu.\n", *((int *)arg), okunan_deger);
    sem_post(sem);

    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_ids[10];

    sem = sem_open(SEM_NAME, O_CREAT, 0644, 0);
    if(sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    if(pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if(pid == 0) {  // Yavru süreç
        close(fd[0]);
        int gonderilen_deger;
        while(1) {
            for(int i = 1; i <= 10; i++) {
                read(STDIN_FILENO, &gonderilen_deger, sizeof(gonderilen_deger));
                write(fd[1], &gonderilen_deger, sizeof(gonderilen_deger));
            }
        }
    } else if(pid > 0) {  // Ana süreç
        close(fd[1]);
        while(1) {
            for(int i = 0; i < 10; i++) {
                thread_ids[i] = i + 1;
                sem_wait(sem);
                pthread_create(&threads[i], NULL, thread_fonksiyonu, &thread_ids[i]);
                pthread_join(threads[i], NULL);
            }
        }
    } else {
        perror("fork");
        exit(1);
    }

    sem_close(sem);
    sem_unlink(SEM_NAME);
    return 0;
}
