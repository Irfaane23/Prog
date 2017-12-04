#include "SDL.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "timer.h"

// Return number of elapsed µsec since... a long time ago
static unsigned long get_time (void)
{
  struct timeval tv;

  gettimeofday (&tv ,NULL);

  // Only count seconds since beginning of 2016 (not jan 1st, 1970)
  tv.tv_sec -= 3600UL * 24 * 365 * 46;

  return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#ifdef PADAWAN


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct event_list{
  void* evenement;
  unsigned long tps;
  struct event_list *next_event;
}LISTE;

LISTE *l = NULL;

LISTE* new_event(long timer,void* event){
  //printf("\non est dans new_event");
  LISTE* lBis = (LISTE*) malloc(sizeof(LISTE));
  //printf("\nmalloc new_event ok");
  lBis->evenement=event;
  lBis->tps=timer;
  lBis->next_event=NULL;
  return lBis;
}

void untilNext(LISTE *l,Uint32 delay, void*param){
  if(l->next_event != NULL){
    printf("\nevenement : %p",l->evenement);
    untilNext(l->next_event,delay,param);
  }
  else{
    l->next_event=new_event(delay,param);
  }
}
void *infiniteLoop(void *p){
    //printf("\ndans infinite loop");
    sigset_t mask;
    //Blocage de tous les signaux
    sigemptyset(&mask);
    //on ajoute au mask le signal SIGALRM : qui esdl_push_eventst son seul signal
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    //printf("\ndans infinite loop, avant le while");
  while(1){
    //printf("\nc'est pas beau wallah");
    sigsuspend(&mask);
  }
}

void handler(int sig){
  printf("Thread courant : %p envoie le signal %d \n", pthread_self(), sig);

  sdl_push_event(&l);
}
// timer_init returns 1 if timers are fully implemented, 0 otherwise
int timer_init (void)
{
  //printf("\ndans timer_init");
  int nbThreads = 3;
  pthread_t t[nbThreads];

  //installation du handler pour le signal SIGALRM
  //printf("\ncréation du sigaction");
  struct sigaction s;
  s.sa_flags = 0;
  sigemptyset(&s.sa_mask);
  s.sa_handler = handler; // va afficher le thread courant
  sigaction(SIGALRM,&s,NULL);
  //printf("\nmask");
  sigset_t block;
  sigemptyset(&block);
  sigaddset(&block, SIGALRM);
  pthread_sigmask(SIG_BLOCK, &block, NULL);

  // Creaation du démon
  //printf("\njuste avant le loop");
 for (int i = 0; i < nbThreads; i++) {
    //printf("\navant le premier create");
    pthread_create(t,NULL,infiniteLoop,NULL);
    //printf("\naprès le premier create");
  }

  return 1; // Implementation not ready
}

timer_id_t timer_set (Uint32 delay, void *param)
{

  //structure du timer

  //printf("\njuste avant itimerval");
  //pthread_mutex_lock(&mutex);
  if(l == NULL){
    //creer un evenement
    //printf("\nadd event");
     l=new_event(delay,param);
  }
  else{
    //on execute le signal
    untilNext(l,delay,param);
    printf ("sdl_push_event(%p) appelée au temps %ld\n", param, get_time ());
  }
  //pthread_mutex_unlock(&mutex);
//  printf ("sdl_push_event(%p) appelée au temps %ld\n", param, get_time ());
  //sdl_push_event();
  //printf("\nadd event");


  ////// PAS TOUCHE /////////////

  struct itimerval timer;
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 0;

  timer.it_interval.tv_sec = delay/1000;
  timer.it_interval.tv_usec = (delay%1000)/1000;
  setitimer(ITIMER_REAL, &timer, NULL);

  return (timer_id_t) NULL;
}

int timer_cancel (timer_id_t timer_id)
{
  //TODO
  return 0; // failure
}

#endif
