/*!
 * \file      timedht.h
 *  
 * Utworzono: 13 lis 2017
 *
 */

#ifndef TIMEDHT_HH
#define TIMEDHT_HH


#include <stdint.h>

 /*!
 *  \brief Plik zawiera definicję funkcje do zmiany priorytetu planisty
 *         i funkcje do odmierzanie czasu
 * 
 *  Funkcje pozwalają na realizację opóźnień w transmisji z czujnikiem
 *  DHT11 i zmianę priotytety planisty do obsługi zliczania impulsów
 *  pochodzących z czujnika DHT11
 *
 */



/*!
 *  \fn schedSetPriority
 * 
 *  \brief Zwiększa priorytet planisty do obsługi zdarzeń FIFO 
 *         (SCHED_FIFO)
 *
 */
void schedSetPriority();


/*!
 *  \fn backNormalPriority
 * 
 *  \brief  Przywraca domyślny priorytet planisty (SCHED_OTHER)
 *
 */
void backNormalPriority();


/*!
 *  \fn sleepms
 * 
 *  \brief  Dokładny pomiar czasu w milisekundach
 *  \param  ms zadana wartość czasu w milisekundach
 *
 */
void sleepms(uint32_t ms);


/*!
 *  \fn delayTransmition
 * 
 *  \brief  Realizuje zadane opóźnienie dla transmisji danych 
 *  \param  ms zadana wartość opóźnienia w milisekundach
 *
 */
void delayTransmition(uint32_t ms);



#endif
