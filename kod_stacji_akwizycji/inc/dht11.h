/*!
 * \file   dht11.h
 *  
 * Utworzono: 13 lis 2017
 *
 */



#ifndef DHT11_HH
#define DHT11_HH



#include "timedht.h"
#include "mmgpio.h"



/*!
 *  \fn read_dht
 * 
 *  \brief Odczytuje wartość temperatury i wilgotności z czujnika DHT11
 * 
 *  \param pinData wskaźnik na strukturę gpio
 *  \param temp wskaźnik na wartość odczytanej temperatury
 *  \param humidity wskaźnik na wartość odczytanej wilgotności
 * 
 *  \retval zwraca 0, gdy nie udało się odczytać temperatury 
 *          i wilgotności; wartość ujemna, gdy nie udało się odczytać 
 *          temperatury i wilgotności
 */
int read_dht(mmgpio *pinData, float* temp, float* humidity);



#endif
