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

void handler(int sig){
  printf("Thread courant : %p envoie le signal %d \n", pthread_self(), sig);
}

void *infiniteLoop(void *p){

    sigset_t mask;
    //Blocage de tous les signaux
    sigemptyset(&mask);
    //on ajoute au mask le signal SIGALRM : qui est son seul signal
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

  while(1){
    sigsuspend(&mask);
  }
}
// timer_init returns 1 if timers are fully implemented, 0 otherwise
int timer_init (void)
{
  int nbThreads = 2; // 1 seul thread
  pthread_t t[nbThreads];



  //installation du handler pour le signal SIGALRM
  struct sigaction s;
  s.sa_flags = 0;
  sigemptyset(&s.sa_mask);
  s.sa_handler = handler; // va afficher le thread courant
  sigaction(SIGALRM,&s,NULL);

  sigset_t block;
  sigemptyset(&block);
  sigaddset(&block, SIGALRM);
  pthread_sigmask(SIG_BLOCK, &block, NULL);

  // Creaation du démon
  for (int i = 0; i < nbThreads; i++) {
    pthread_create(t,NULL,infiniteLoop,NULL);
  }

  //on attend que les threads terminent
  for (int i = 0; i < nbThreads; i++) {
    pthread_join(t[i],NULL);
  }

  return 0; // Implementation not ready
}

timer_id_t timer_set (Uint32 delay, void *param)
{
  // TODO

  printf ("sdl_push_event(%p) appelée au temps %ld\n", param, get_time ());
  //sdl_push_event();
  return (timer_id_t) NULL;
}

int timer_cancel (timer_id_t timer_id)
{
  // TODO

  return 0; // failure
}

#endif
