#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//Producer consumer example.
//Consumer can only consume items after the consumer has made items
int full;
int count;

//Conditional Variables
//Used for handshaking
pthread_cond_t cond_recv=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_send=PTHREAD_COND_INITIALIZER;

//Mutex
pthread_mutex_t cond_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t count_mutex=PTHREAD_MUTEX_INITIALIZER;

void *consumer (void *in) {

  while (1) {
    pthread_mutex_lock(&cond_mutex);
    
    //Wait for other
    while(!full) {
      pthread_cond_wait(&cond_send,&cond_mutex);
    }

    pthread_mutex_unlock(&cond_mutex);
    pthread_mutex_lock(&count_mutex);
    full=0;
    printf("Consumed():%d\n",count);
    pthread_cond_broadcast(&cond_recv);
    pthread_mutex_unlock(&count_mutex);
    if (count>10) break;
  }
  pthread_exit(NULL);

}

void *producer(void *in) {
  while(1) {
    pthread_mutex_lock(&cond_mutex);

    //Wait for other
    while(full) {
      pthread_cond_wait(&cond_recv,&cond_mutex);
    }

    pthread_mutex_unlock(&cond_mutex);
    pthread_mutex_lock(&count_mutex);
    count++;full=1;
    printf("Produced():%d\n",count);
    pthread_cond_broadcast(&cond_send);
    pthread_mutex_unlock(&count_mutex);
    if(count>10) break;
  }
  pthread_exit(NULL);


}


int main (void) {
  pthread_t cons_thread,prod_thread;
  pthread_create(&prod_thread,NULL,producer,NULL);
  pthread_create(&cons_thread,NULL,consumer,NULL);

  pthread_join(cons_thread, NULL);
  pthread_join(prod_thread, NULL);
  return 0;
} //main
