#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NELEMENTS 5000
#define BLK_SIZE 1000
#define NUM_THREADS (NELEMENTS/BLK_SIZE)

void *doWork( void *threadid ) {
  long tid;

  tid = (long)threadid;
  printf("Hello World from thread %ld\n",tid);
  pthread_exit(0);
}

int main (void) {
  
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;


  int rc;
  long t;
  void *status;

  //Initialize the attr
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for(t=0; t<NUM_THREADS; t++) {
    printf("In main, creating thread id: %ld\n",t);
    rc=pthread_create(&threads[t], &attr, doWork, (void *) t);
    if(rc) {
      printf("ERROR; return code from pthread_create: %d\n",rc);
      exit(-1);
    }
  } //threads

  //Free attribure
  pthread_attr_destroy(&attr);

  for(t=0; t<NUM_THREADS; t++) {
    rc = pthread_join(threads[t], &status);
    if (rc) {
      printf("ERROR! Thread returned w/ following status %d\n",rc);
    }
  }

  printf("Main: Complete\n");


} //main
