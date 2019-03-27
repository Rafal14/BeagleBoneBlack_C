/*!
 * \file   timedht.c
 *  
 * Utworzono: 13 lis 2017
 *
 */


#include <sys/time.h>
#include <time.h>
#include <sched.h>
#include <errno.h>
#include <string.h>


#include "timedht.h"


/* Ustawia (zwieksza) priorytet planisty */
void schedSetPriority() {
	struct sched_param scheduler;
	memset(&scheduler, 0, sizeof(scheduler));
	
	scheduler.sched_priority = sched_get_priority_max(SCHED_FIFO);
	sched_setscheduler(0, SCHED_FIFO, &scheduler);
	
}



/* powrot do domyslnego priorytety planisty */
void backNormalPriority() {
	struct sched_param scheduler;
	memset(&scheduler, 0, sizeof(scheduler));
	scheduler.sched_priority = 0;
	sched_setscheduler(0, SCHED_OTHER, &scheduler);
}



/* pomiar czasu w ms */
void sleepms(uint32_t ms) {
	struct timespec lag;
	lag.tv_sec  = ms / 1000;
	lag.tv_nsec = (ms % 1000) * 1000000L;
	while (clock_nanosleep(CLOCK_MONOTONIC, 0, &lag, &lag) 
	                                              && errno == EINTR);
}


/* opoznienie dla realizacji transmisji */
void delayTransmition(uint32_t ms) {	
	
	/* definiuj struktury dla deklarowanej i zmierzonej wartości czasu*/
	struct timeval diftime, currtime, endtime;

	diftime.tv_sec  = ms / 1000;   /* zamień sekundy na milisekundy */
	diftime.tv_usec = (ms % 1000) * 1000;

	/* pobierz wartość bieżącego czasu */
	gettimeofday(&currtime, NULL);
  
	/* oblicz wartość końcową czasu */
	timeradd(&currtime, &diftime, &endtime);
	
	/* sprawdź czy odmierzono zadeklarowany czas */
	while (timercmp(&currtime, &endtime, <)) {
		/* pobierz wartość bieżącego czasu */
		gettimeofday(&currtime, NULL);
	}
  
}
