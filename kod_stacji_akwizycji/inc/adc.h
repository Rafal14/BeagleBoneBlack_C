/*!
 * \file adc.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 21 paź 2017
 *
 */

#ifndef ADC_H
#define ADC_H



/*!
 *  \file Definicja funkcji dostępu do przetwornika analogowo-cyfrowego
 *
 *  Plik zawiera definicję funkcji obsługi przetwornika
 *  analogowo-cyfrowego
 *
 */


#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*!
 *  \fn enableAdc
 * 
 *  \brief Uaktywania przetwornik analogowo-cyfrowy

 *  \retval zwraca wartość 0, gdy operacja się powiodła;
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int enableAdc();


/*!
 *  \fn adcCheckDriverNo
 * 
 *  \brief Sprawdza numer sterownika adc
 *  \retval zwraca number sterowanika adc w przypadku powodzenia
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int adcCheckDriverNo();


/*!
 *  \fn adcReadValue
 * 
 *  \brief Odczytuje wartość z pinu przetwornika analogowo-cyfrowego
 *
 *  \param[in]  określa number sterownika
 *  \param[in]  number określa number pinu ADC
 *
 *  \retval    zwaraca wartość 0, gdy wartość została odczytana
 *             zwraca -1 w przeciwnym wypadku
 *
 */
int adcReadValue(int driver, unsigned int number);



#endif
