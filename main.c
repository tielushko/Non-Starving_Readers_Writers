#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define NTHREADS 10 //NOTE: keep the number of threads in the scenario under 10, if want to increase, modify this

/**************************************************************************************************************
* the Reader and Writer acquire and release code is a repurposed code from the OSTEP textbook chp. 31 code,
* reading_writing() is a function from the project prompt. The code also 
* includes the addition of another semaphore wIn to create a queue for threads to avoid writing starvation. The 
* idea for this was researched in the 'The Little Book On Semaphores' and implemented by me.
***************************************************************************************************************/

//the struct for reader-writer locks
typedef struct {
    sem_t wrt; //write lock - to prevent a writer from entering the system when readers are there.
    sem_t mutex; //mutex - to prevent threads from modifying counter.
    int counter; // the number of readers in the execution.
    sem_t wIn; //turnslite semaphore, once writer thread enter, it calls sem_wait on the wIn, so when the reader arrives after, it puts itself to sleep, which allows the writer to not get overcome by readers and prevents starvation.
} rwlock_t; 


//initialization of reader-writer lock
void rwlock_init(rwlock_t *rw) {
    rw->counter = 0;
    sem_init(&rw->wrt, 0, 1);
    sem_init(&rw->mutex, 0, 1); 
    sem_init(&rw->wIn, 0, 1);
}

//function to be executed in the CS of a reader and writer.
void reading_writing(){
    int x = 0, T;
    int i, j;
        T = rand() % 10000;
        for(i = 0; i < T; i++)
            for(j = 0; j < T; j++)
                x = i * j;
    return;
}


//acquire read for the reader: 
void acquire_read(rwlock_t *rw) {
    sem_wait(&rw->wIn); 
    sem_wait(&rw->mutex); 
    rw->counter += 1;
    //reader entered
    printf("Reader Entered. No writer is allowed when readers are present.\n");
    if (rw->counter == 1) {
       sem_wait(&rw->wrt);
    } 
    sem_post(&rw->mutex);
    sem_post(&rw->wIn);
}

//when "reading" is done, release read
void release_read(rwlock_t *rw) {
    
    sem_wait(&rw->mutex); 
    rw->counter -= 1; 
    if (rw->counter == 0) {
        sem_post(&rw->wrt);
        printf("Last reader is about to be out.\n");
    }
    sem_post(&rw->mutex);
}

//acquire writer locks
void acquire_write(rwlock_t *rw) {
    sem_wait(&rw->wIn);
    sem_wait(&rw->wrt);
    printf("Writer Entered, no other writers are allowed\n");
}

//release writer lock
void release_write(rwlock_t *rw) {
    sem_post(&rw->wrt);
    sem_post(&rw->wIn);
}  


//read-write lock
rwlock_t rw;


//wasting time function for reader with lock calls.
void* reader(void *args) {
    acquire_read(&rw);
    //critical section goes here.
    reading_writing();
    //end crit section here.
    release_read(&rw);
    printf("Read done\n"); 
    return NULL;
}

//wasting time function for writer with lock calls.
void* writer(void *args) {
   
    acquire_write(&rw);
    //critical section goes here.
    reading_writing();
    //end crit section here.
    release_write(&rw);
    printf("Write done\n");
    return NULL;
} 

//main.
int main() {
  
    //initialize our lock's semaphores and counter.
    rwlock_init(&rw); 
    
    //file pointer and error-handling.
    FILE* fp;
    fp = fopen("scenarios.txt", "r");
    
    if (fp == NULL) {
      fprintf(stderr, "Error opening the file"); 
    }
    
    /*creating an array of threads, each line will represent and array of threads created. The limit is 10
      threads per scenario. The scanner will go through the file, scan the line first and place the values 
      into an array of symbols, then it will go and through the array of them and create appropriate threads
      after the readers/writers are executed, it will then join the threads back together
    */
    
    pthread_t threads[NTHREADS];
    char symbols[NTHREADS];
    int index = 0;

    while (1){
      //scan a single character
      char c = fgetc(fp);
      
      //if end of file, we exit the while loop and finish the program
      if(c == EOF) {
          printf("End of file\n");
          break;
      } else if (c == '\n') { //if the end of line, we initialize the threads we have in our array of threads 
                              //with the functions we want them to execute based on the symbols we have in array
                              //of symbols.
          index = 0;
          //do all the threads
          int i;
          printf("\nScenario begins!\n");
          for (i = 0; i < NTHREADS; i++) {
            //make appropriate function call based on symbol scanned
            if(symbols[i] == 'r') {
               if (pthread_create(&threads[i], NULL, reader, NULL) != 0) {
                  fprintf(stderr, "Error: Cannot create thread # %d\n", i);
                  break;
               }
            } else if(symbols[i] == 'w') {
                if (pthread_create(&threads[i], NULL, writer, NULL) != 0) {
                  fprintf(stderr, "Error: Cannot create thread # %d\n", i);
                  break;
               }
            } else { //error-handling
              fprintf(stderr, "Not a read or write symbol entered in the text file!\n");
            }
          }
          
          //joining the threads after their execution with error-handling
          for (i = 0; i < NTHREADS; i++) {
            if (pthread_join(threads[i], NULL) != 0) {
              fprintf(stderr, "Error: Cannot join thread # %d\n", i);
            }
          }
          printf("Scenario Done!\n\n");
          continue;
      } else if(isspace(c)) { //ignore whitespace chars
            continue;
      } else if (c == 'w' || c == 'r' ){   //fill up the symbol array upon seeing either r or w  
          //fill up the symbol array by copying a c and then moving the index forward
          symbols[index] = c;
          printf("%c ", symbols[index]); //line of code for debug to see if the array of symbols gets populated.
          index++;
      } else { //bleh
        fprintf(stderr, "Incorrect Symbol");
      }
    }
    
    printf("all done\n");
    return 0;
}