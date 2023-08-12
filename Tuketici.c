#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <signal.h>


int fd[2];  

void *thread_fonksiyonu(void *arg) {
    int okunan_deger = *((int*) arg);
    write(fd[1], &okunan_deger, sizeof(okunan_deger));  
    sleep(1);
    return NULL;
}


sem_t *sem_for_tuketici;
sem_t *sem_for_uretici;

// this is to reset the semaphores 
void handle_signal(int signal) {
    sem_close(sem_for_tuketici);
    sem_unlink("/sem_for_tuketici");
    sem_close(sem_for_uretici);
    sem_unlink("/sem_for_uretici");
    exit(0);
}


int main() {

    signal(SIGINT, handle_signal);

    pthread_t threads[10];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) {  
        close(fd[1]);
        int okunan_deger;
        while (1) {
            read(fd[0], &okunan_deger, sizeof(okunan_deger));
            printf("Thread: %d okundu.\n", okunan_deger);
        }
    } else if (pid > 0) {  
        close(fd[0]);

        sem_t *sem_for_tuketici = sem_open("/sem_for_tuketici", O_CREAT, 0644, 0);
        sem_t *sem_for_uretici = sem_open("/sem_for_uretici", O_CREAT, 0644, 1);

        int counter = 0;
        while (1) {
            sem_wait(sem_for_tuketici);  
            for (int i = 0; i < 10; i++) {
                counter++;
                pthread_create(&threads[i], NULL, thread_fonksiyonu, &counter);
                pthread_join(threads[i], NULL);
            }
            counter = 0;
            sem_post(sem_for_uretici);
        }

        sem_close(sem_for_tuketici);
        sem_unlink("/sem_for_tuketici");
        sem_close(sem_for_uretici);
        sem_unlink("/sem_for_uretici");
    }

    return 0;
}
