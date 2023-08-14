Üretici ve Tüketici Problem
This repository contains an implementation of the classic Producer-Consumer problem using semaphores and threads in C.

Overview
The Producer-Consumer problem is a classic synchronization problem which demonstrates multi-process synchronization using shared resources. In our implementation:

Üretici (Producer) produces some items and then sleeps.
Tüketici (Consumer) consumes those items and then sleeps.
Both processes use semaphores to synchronize their actions and ensure that the producer doesn't produce more items when a certain number have not been consumed, and that the consumer doesn't consume items that haven't been produced.

Files
Uretici.c: Contains the producer logic. The producer produces items and then signals the consumer to start consuming.

Tuketici.c: Contains the consumer logic. The consumer waits for the producer signal to start consuming the items.

Usage
Compile the source files:
bash
Copy code
gcc Uretici.c -o Uretici -lpthread -lrt
gcc Tuketici.c -o Tuketici -lpthread -lrt
Run the compiled binaries in separate terminal windows or processes:
bash
Copy code
./Uretici
./Tuketici
Clean-up
If you stop the program using CTRL + C and face issues when starting it again, you might need to manually remove the semaphores:

bash
Copy code
rm /dev/shm/sem_for_tuketici
rm /dev/shm/sem_for_uretici
Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
