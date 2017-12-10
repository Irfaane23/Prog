#include "SDL.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "timer.h"


static unsigned long get_time (void)
{
  struct timeval tv;

  gettimeofday (&tv ,NULL);

  tv.tv_sec -= 3600UL * 24 * 365 * 46;

  return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#ifdef PADAWAN


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct event_list{
  void* evenement;
  unsigned long tps;
  struct event_list *next_event;
  unsigned long time;
}LISTE;

LISTE *l = NULL;

LISTE* new_event(long timer,void* event){
  LISTE* lBis = (LISTE*) malloc(sizeof(LISTE));
  lBis->evenement=event;
  lBis->tps=timer;
  lBis->next_event=NULL;
  lBIS->time=get_time();
  return lBis;
}

void untilNext(LISTE *l,Uint32 delay, void*param){
  if(l->next_event != NULL){
    printf("\nevenement : %p",l->evenement);
    untilNext(l->next_event,delay+get_time(),param);
  }
  else{
    l->next_event=new_event(delay+get_time(),param);
    printf("\nnext_event : %p",l->next_event);
  }
}


void *infiniteLoop(void *p){
    sigset_t mask;
    sigemptyset(&mask);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
  while(1){
    sigsuspend(&mask);

  }
}

void handler(int sig){
    sdl_push_event(l->evenement);
    l=l->next_event;
  }
int timer_init (void)
{
  int nbThreads = 5;
  pthread_t t[nbThreads];
  struct sigaction s;
  s.sa_flags = 0;
  sigemptyset(&s.sa_mask);
  s.sa_handler = handler;
  sigaction(SIGALRM,&s,NULL);
  sigset_t block;
  sigemptyset(&block);
  sigaddset(&block, SIGALRM);
  pthread_sigmask(SIG_BLOCK, &block, NULL);


 for (int i = 0; i < nbThreads; i++) {
    pthread_create(t,NULL,infiniteLoop,NULL);
  }

  return 1;
}

timer_id_t timer_set (Uint32 delay, void *param)
{
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;

  timer.it_value.tv_sec = delay/1000;
  timer.it_value.tv_usec = (delay%1000)*1000;


  if(l == NULL){
     l=new_event(get_time()+delay,param);
  }
  else{

    timer.it_value.tv_sec = (delay-(get_time()-l->time))/1000;
    timer.it_value.tv_usec = ((delay-(get_time()-l->time))%1000)*1000;
    setitimer(ITIMER_REAL,&timer, NULL)
    untilNext(l,delay +get_time(),param);

  }
  pthread_mutex_lock(&mutex);
  setitimer(ITIMER_REAL, &timer, NULL);
  pthread_mutex_unlock(&mutex);
  return (timer_id_t) NULL;
}

int timer_cancel (timer_id_t timer_id)
{
  return 0;
}

#endif
