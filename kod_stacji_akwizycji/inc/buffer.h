/*!
 * \file   buffer.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 30 paź 2017
 *
 */


#ifndef BUFFER_H
#define BUFFER_H


/*!
 *  \brief Definicja struktury value_t i struktury buffer_t
 *
 *  Plik zawiera definicję struktury value_t, która przechowuje wartości
 *  pomiarowe oraz definicję strukturę buffer_t określącej bufor
 *  cykliczny.
 *
 */


#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



#define BUFSIZE 4


/*!
 *  \struct value_t
 *  \brief Struktura zawiera tablicę stanów wejść i wyjść cyfrowych, 
 *         tablicę rejestrów wejściowych i tablicę pamięci 
 *
 *  \param inputDiscrete     tablicę stanów wejść cyfrowych
 *  \param outputDiscrete    tablicę stanów wyjść cyfrowych
 *  \param inputRegisters    tablicę zawierająca wartości rejestrów
 *                           wejściowych - wartości wejść analogowych
 *  \param holdRegisters     tablica zawierająca przetworzone wartości
 *                           pomiarowe - obliczone wartości z czujników
 *                           cyfrowych
 *
 */
typedef struct value_t {
	uint8_t  inputDiscrete[5];  
	uint8_t  outputDiscrete[6];   
	uint16_t inputRegisters[32];  
	uint32_t holdRegisters[32];
	
} value_t;



/*!
 *  \struct buffer_t
 *  \brief Struktura definiująca bufor cykliczny
 *
 *  \param buf    tablicę pomiarów o rozmiarze BUFSIZE
 *  \param head   indeks do zapisu wartości w buforze
 *  \param tail   indeks do odczytu wartości z bufora
 *  \param mutex  semafor nienazwany do blokowania dostępu do bufora
 *                w czasie odczytu i zapisu 
 */
typedef struct buffer_t {
	
	value_t buf[BUFSIZE];
		
	int     head;           
	int     tail;          
	sem_t   mutex;    

	
} buffer_t;



/*!
 *  \fn initBuf
 * 
 *  \brief  Inicjalizuje wartości indeksów bufora cyklicznego
 *
 *  \param  circularBuf       wskaźnik na tablicę bufora
 */
void initBuf(buffer_t *circularBuf);


/*!
 *  \fn isEmpty
 * 
 *  \brief  Sprawdza czy bufor nie jest pusty
 *
 *  \param  circularBuf       wskaźnik na tablicę bufora
 * 
 *  \retval zwraca 0 gdy bufor jest pusty, -1 w przeciwnym wypadku
 */
int isEmpty(buffer_t *circularBuf);



/*!
 *  \fn addElem
 * 
 *  \brief  Dodaje element zawierający wartości pomiarowe do bufora 
 *          cyklicznego
 *  \param  circularBuf       wskaźnik na tablicę bufora
 *  \param  elem              element pomiarowy, zawiera bieżące 
 *                            wartości pomiarowe
 */
void addElem(buffer_t *circularBuf, value_t elem);


/*!
 *  \fn readElem
 * 
 *  \brief  Odczytuje element zawierający wartości pomiarowe z bufora 
 *          cyklicznego
 *  \param  circularBuf       wskaźnik na tablicę bufora
 *  \param  element           wskaźnika na wartość elementu pomiarowego
 * 
 * 	\retval zwraca 0 gdy operacja się powiodła, -1 w przeciwnym wypadku
 */
int readElem(buffer_t *circularBuf, value_t *element);






#endif
