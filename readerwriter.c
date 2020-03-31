#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

