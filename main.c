#include <stdio.h>
#include "sbuffer.h"
#include <pthread.h>
#include <sys/stat.h>

#define FIFO_NAME "logFifo"

int counter;

void *trythis(void *arg) {


    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d has started\n", counter);

    for (i = 0; i < (0xFFFFFFFF); i++);

    printf("\n Job %d has finished\n", counter);


    return NULL;
}

int main() {
    int error = mkfifo(FIFO_NAME, S_IRUSR | S_IWUSR);
    (void) error;

    sbuffer_t *buffer;
    sbuffer_init(&buffer);

    pthread_t thread1;
    error = pthread_create(&thread1, NULL, trythis, buffer);

    pthread_t thread2;
    error = pthread_create(&thread2, NULL, trythis, buffer);

    sbuffer_wait_readers(buffer, 2);

    sbuffer_close(buffer);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sbuffer_free(&buffer);
    return 0;
}
