#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *printHello( void *threadid ) {
  long tid;

  tid = (long)threadid;
  printf("Hello World from thread %ld\n",tid);
  pthread_exit(NULL);
}

int main (void) {
  
  pthread_t threads[NUM_THREADS];

  int rc;
  long t;

  for(t=0; t<NUM_THREADS; t++) {
    printf("In main, creating thread id: %ld\n",t);
    rc=pthread_create(&threads[t], NULL, printHello, (void *) t);
    if(rc) {
      printf("ERROR; return code from pthread_create: %d\n",rc);
      exit(-1);
    }
  } //threads

  /* pthread_exit(NULL); */


} //main
