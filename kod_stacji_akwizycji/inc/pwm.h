/*
 * \file pwm.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 21 paź 2017
 *
 */

#ifndef PWM_H
#define PWM_H





/*!
 *  \file Definicja struktury pwm i funkcji działających 
 *        na tej strukturze
 *
 *  Plik zawiera definicję struktury pwm i funkcji 
 *  obsługi PWM
 *
 */


#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>





/*!
 *  \struct pwm
 *  \brief Struktura definiująca wyjście PWM
 *
 *  \param socketNo określa number gniazda (8 lub 9) na płytce PCB 
 *  \param number   określa numer pinu w gnieździe
 *  \param driver   określa numer sterownika
 *  \param period   określa okres sygnału PWM [ns]
 *
 */

typedef struct pwm
{
	int  socketNo;
	int  number;
	int  driver;
	long period;
	
} pwm;



/*!
 *  \fn enablePwmControl
 * 
 *  \brief Uaktywania sterowanie PWM
 *
 *  \retval zwraca wartość 0, gdy operacja się powiodła
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int enablePwmControl();


/*!
 *  \fn enablePwmPin
 * 
 *  \brief Uaktywania określony pin PWM
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  portNo określa number gniazda (8 lub 9) na płytce PCB 
 *  \param  pinNo określa numer pinu w gnieździe
 *  \param  driverNo   określa numer sterownika
 *  \retval zwaraca wartość 0, gdy pin pwm został uaktywniony;
 *          zwraca -1, gdy aktywacja pinu pwm się nie powiodła
 *
 */
int enablePwmPin(pwm *pw, unsigned int portNo, unsigned int pinNo);


/*!
 *  \fn pwmCheckDriverNo
 * 
 *  \brief Sprawdza numer sterownika określonego pinu PWM
 *  \retval zwraca number sterowanika określonego pinu PWM
 *          zwraca -1 w wypadku niepowodzenia
 *
 */
int pwmCheckDriverNo(pwm *pw);



/*!
 *  \fn pwmSetPolarity
 * 
 *  \brief Zmienia polaryzację określonego pinu PWM
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  polarity określa polaryzacje sygnału PWM: 
 *          0 - sygnał rozpoczyna się od 0V
 *          1 - sygnał rozpoczyna się od 3.3V 
 *  \retval zwraca wartość 0, gdy zmiana polaryzacji się powiodła
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int pwmSetPolarity(pwm *pw, unsigned int polarity);


/*!
 *  \fn pwmGetPolarity
 * 
 *  \brief Sprawdza polaryzację określonego pinu PWM
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  polarity określa polaryzacje sygnału PWM: 
 *          0 - sygnał rozpoczyna się od 0V
 *          1 - sygnał rozpoczyna się od 3.3V 
 *  \retval zwraca wartość 0, gdy pobrano wartość określającą 
 *          polaryzacje, zwraca -1 w przeciwnym wypadku
 *
 */
int pwmGetPolarity(pwm *pw, unsigned int *polarity);


/*!
 *  \fn pwmSetPeriod
 * 
 *  \brief Zmienia okres sygnału PWM, wystawianego na określony pin
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  timePeriod zadana wartość okresu sygnału PWM [ns]
 * 
 *  \retval zwraca wartość 0, gdy zmiana okresu sygnału PWM się powiodła
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int pwmSetPeriod(pwm *pw, long timePeriod);


/*!
 *  \fn pwmGetPeriod
 * 
 *  \brief Odczytuje wartość okresu sygnału PWM, wystawianego 
 *         na określony pin
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  timePeriod wskaźnik na wartość określającą okres PWM
 * 
 *  \retval zwraca wartość 0, gdy odczyt wartości sygnału PWM 
 *          się powiodła, zwraca -1 w przeciwnym wypadku
 *
 */
int pwmGetPeriod(pwm *pw, long *timePeriod);



/*!
 *  \fn pwmSetTimeDuty
 * 
 *  \brief Zmienia wypełnienie sygnału PWM, wystawianego 
 *         na określony pin
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  timeDuty zadana wartość wypełnienia sygnału PWM [ns]
 * 
 *  \retval zwraca wartość 0, gdy zmiana wypełnienia sygnału PWM 
 *          się powiodła, zwraca -1 w przeciwnym wypadku
 *
 */
int pwmSetTimeDuty(pwm *pw, long timeDuty);


/*!
 *  \fn pwmGetTimeDuty
 * 
 *  \brief Odczytuje wartość wypełnienia okresu sygnału PWM
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  timeDuty wskaźnik na odczytaną wartość wypełnienia PWM [ns]
 * 
 *  \retval zwraca wartość 0, gdy odczytano wartość wypełnienia sygnału 
 *          PWM się powiodła, zwraca -1 w przeciwnym wypadku
 *
 */
int pwmGetTimeDuty(pwm *pw, long *timeDuty);


/*!
 *  \fn pwmSetDutyCycle
 * 
 *  \brief Zmienia wypełnienie sygnału PWM, wystawianego 
 *         na określony pin
 *
 *  \param pw wskaźnik na strukturę pwm
 *  \param timeDuty zadana wartość wypełnienia sygnału PWM 
 *         jako stosunek wartości wypełnienia [ns] do okresu sygnału[ns]
 * 
 *  \retval zwraca wartość 0, gdy zmiana wypełnienia sygnału PWM 
 *          się powiodła, zwraca -1 w przeciwnym wypadku
 *
 */
int pwmSetDutyCycle(pwm *pw, float dutyCycle);



/*!
 *  \fn pwmGetDutyCycle
 * 
 *  \brief Odczytuje wartość wypełnienia okresu sygnału PWM
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  timeDuty wskaźnik na odczytaną wartość wypełnienia PWM
 *        jako stosunek wartości wypełnienia [ns] do okresu sygnału[ns]
 * 
 *  \retval zwraca wartość 0, gdy odczytano wartość wypełnienia sygnału 
 *          PWM się powiodła, zwraca -1 w przeciwnym wypadku
 *
 */
int pwmGetDutyCycle(pwm *pw, float *dutyCycle);


/*!
 *  \fn pwmRun
 * 
 *  \brief Włącza lub wyłącza sygnał PWM zadawany na określony pin
 *
 *  \param  pw wskaźnik na strukturę pwm
 *  \param  mode określa czy sygnał ma zostać wyłączony (0) 
 *          lub załączony (1)
 * 
 *  \retval zwraca wartość 0, gdy załączenie/wyłączenie pinu PWM
 *          się powiodło, zwraca -1 w przeciwnym wypadku
 *
 */
int pwmRun(pwm *pw, unsigned int mode);



#endif 
