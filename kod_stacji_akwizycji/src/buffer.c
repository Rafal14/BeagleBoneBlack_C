



#include "buffer.h"




void initBuf(buffer_t *circularBuf) 
{
	circularBuf->head  = 0;
	circularBuf->tail  = 0;
}


int isEmpty(buffer_t *circularBuf) 
{
	return (circularBuf->head == circularBuf->tail) ? 0 : -1;
}



void addElem(buffer_t *circularBuf, value_t elem) 
{
	int headInd = circularBuf->head;
	int nextInd = circularBuf->head +1;

	circularBuf->buf[headInd] = elem;    /* zapisz element w buforze */
	
	/* ustalenie wartości indeksu następnego elementu do zapis */
	nextInd = nextInd % BUFSIZE;
	
	circularBuf->head = nextInd;        /* ustal nowy indeks dla głowy */
	
	/* sprawdz czy nie nastapiło przekroczenie indeksu ogona */
	if ( circularBuf->head == circularBuf->tail ) {
		/* przesun indeks czytający */
		circularBuf->tail = (circularBuf->tail + 1) % BUFSIZE;
	}
}


int readElem(buffer_t *circularBuf, value_t *element)
{	
	int endInd  = circularBuf->tail;
	int nextInd = endInd + 1;

	/* sprawdzenie czy bufor jest pusty */
	if ( isEmpty(circularBuf) == 0 ) {
		return -1;                 
	}
	
	nextInd = nextInd % BUFSIZE;

	*element = circularBuf->buf[endInd];
	
	circularBuf->tail = nextInd;
	
	return 0;
}
