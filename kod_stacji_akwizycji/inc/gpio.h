/*!
 * \file gpio.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 21 paź 2017
 *
 */

#ifndef GPIO_H
#define GPIO_H

/*!
 *  \file Definicja struktury gpio i funkcji działających 
 *        na tej strukturze
 *
 *  Plik zawiera definicję struktury gpio i funkcji 
 *  pozwalających na skonfigurowanie wejść/wyjść 
 *  cyfrowych (gpio)
 *
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>


#define MAX_EVENTS_BUF 30


/*!
 *  \struct gpio
 *  \brief Struktura opisująca wejście/wyjście cyfrowe typu gpio
 *
 *  \param number określa numer pinu typu gpio
 *  \param timeout określa czas odpytywania pinu (domyśnie wyłączony)
 *  \param events bufor na zdarzenia w trybie odpytywania
 *
 */
typedef struct gpio
{
	unsigned int number;
	int          timeout;
	char         events[MAX_EVENTS_BUF];        

} gpio;


/*!
 *  \fn gpioEnable
 * 
 *  \brief Uaktywania określony pin typu gpio
 *
 *  \param  gp wskaźnik na strukturę gpio
 *  \param  num  określa numer pinu
 *  \param  pollingTime czas odpytywania (domyślnie wyłączone)
 *  \retval zwaraca wartość 0, gdy pin gpio został uaktywniony;
 *          zwraca -1, gdy aktywacja pinu gpio się nie powiodła
 *
 */
int gpioEnable(gpio *gp, unsigned int num, int pollingTime);


/*!
 *  \fn gpioDisable
 * 
 *  \brief  Dezaktywuje określony pin typu gpio
 *
 *  \param  gp wskaźnik na strukturę gpio
 *  \retval zwaraca wartość 0, gdy pin gpio został dezaktywniony;
 *          zwraca -1, gdy dezaktywacja pinu gpio się nie powiodła
 *
 */
int gpioDisable(gpio *gp);


/*!
 *  \fn gpioSetDirection
 * 
 *  \brief Ustawia kierunek określonego pin typu gpio
 *
 *  Ustawia pin gpio jako wejście lub wyjście.
 *
 *  \param[in]  gpio wskaźnik na strukturę gpio
 *  \retval    zwaraca wartość 0, gdy został ustalony kierunek pinu
 *             zwraca -1, gdy określenie kierunku pinu się nie powiodło
 *
 */
int gpioSetDirection(gpio *gp, char *dir);


/*!
 *  \fn gpioGetDirection
 * 
 *  \brief Pobiera ustaloną wartość kierunku pin typu gpio
 *
 *  Sprawdza kierunek pinu typu gpio.
 *
 *  \param[in]   gpio wskaźnik na strukturę gpio
 *  \param[out]  tablica znaków okręślająca kierunek pinu
 *
 *  \retval    zwaraca wartość 0, gdy został ustalony kierunek pinu
 *             zwraca -1, gdy określenie kierunku pinu się nie powiodło
 *
 */
int gpioGetDirection(gpio *gp, char *dir);


/*!
 *  \fn gpioSetValue
 * 
 *  \brief Ustawia wartość żądaną na pinie gpio
 *
 *  Ustawia logiczne "0" lub logiczną "1" na pinie gpio
 *
 *  \param  gpio wskaźnik na strukturę gpio
 *  \param  value określa żadaną wartość pinu
 *  \retval zwaraca wartość 0, gdy ustawiono żądaną wartość na pinie
 *          zwraca -1, gdy ustawienie wartości pinu się nie powiodło
 *
 */
int gpioSetValue(gpio *gp, unsigned int value);


/*!
 *  \fn gpioGetValue
 * 
 *  \brief Pobiera wartość z określonego pinu gpio
 *
 *  Pobiera wartość logiczną z określonego pinu gpio
 *
 *  \param  gpio wskaźnik na strukturę gpio
 *  \param  value wskaźnik na pobraną wartość pinu
 *  \retval zwaraca wartość 0, gdy pobrano wartość z pinu
 *          zwraca -1, gdy pobranie wartości z pinu się nie powiodło
 *
 */
int gpioGetValue(gpio *gp, unsigned int *value);



/*!
 *  \fn gpioSetEdge
 * 
 *  \brief Ustawia typ wyzwalania pinu
 *
 *  Ustawia typ wyzwalania pinu wejścia
 *
 *  \param  gpio wskaźnik na strukturę gpio
 *  \param  edge określa typ wyzwalania wejścia, możliwe typy:
 * 			rising - wyzwalanie zboczem narastającym
 * 			falling- wyzwalanie zboczem opadającym
 *          both   - wyzwalanie na oba zbocza
 *
 *  \retval zwaraca wartość 0, gdy ustawiono wyzwalanie wejścia
 *          zwraca -1, gdy ustawienie wyzwalania nie powiodło
 *
 */
int gpioSetEdge(gpio *gp, char *edge);


/*!
 *  \fn gpioGetEdge
 * 
 *  \brief Sprawdza typ wyzwalania pinu
 *
 *  Sprawdza typ wyzwalania pinu wejściowego
 *
 *  \param[in]   gpio wskaźnik na strukturę gpio
 *  \param[out]  edge określa typ wyzwalania wejścia, możliwe typy:
 * 			     rising - wyzwalanie zboczem narastającym
 * 			     falling- wyzwalanie zboczem opadającym
 *               both   - wyzwalanie na oba zbocza
 *
 *  \retval zwaraca wartość 0, gdy ustawiono wyzwalanie wejścia
 *          zwraca -1, gdy ustawienie wyzwalania nie powiodło
 *
 */
int gpioGetEdge(gpio *gp, char *edge);


/*!
 *  \fn gpioActiveLowChange
 * 
 *  \brief Zmienia stan aktywacji pinu
 * 
 *  Zmienia stan aktywacji pinu, gdy wartość wpisywana do pliku
 *  active_low jest dodatnia
 * 
 *  \param[in]   gpio  wskaźnik na strukturę gpio
 *  \param[out]  value gdy dodatnia sterownik zmienia stan aktywacji
 *               pinu gpio
 * 
 *  \retval zwaraca wartość 0, gdy udało się zmienić stan aktywacji gpio
 *          zwraca -1, gdy nie udało się zmienić stanu aktywacji pinu
 * 
 */
 int gpioActiveLowChange(gpio *gp, unsigned int value);
 
 
/*!
 *  \fn gpioActiveLowRead
 * 
 *  \brief Odczytuje wartość stanu aktywacji pinu
 * 
 *  Odczytuje wartość stanu aktywacji pinu ("0" lub "1")
 * 
 *  \param[in]   gpio  wskaźnik na strukturę gpio
 *  \param[out]  value gdy dodatnia sterownik zmienia stan aktywacji
 *               pinu gpio
 * 
 *  \retval zwaraca wartość 0, gdy powiodło się odczytanie wartości
 *          zwraca -1, gdy nie powiodło się odczytanie wartości
 * 
 */
 int gpioActiveLowRead(gpio *gp, unsigned int *value);
 
  
/*!
 *  \fn gpioPoll
 * 
 *  \brief Odpytywanie pinu gpio
 * 
 *  Sprawdza wartość pinu w trybie odpytywania
 * 
 *  \param[in] gp wskaźnik na strukturę gpio
 * 
 *  \retval zwaraca wartość 0, gdy udało się ustawić tryb odpytywania
 *          zwraca -1, gdy nie udało się ustawić trybu odpytywania
 */ 
int gpioPoll(gpio *gp);


#endif 
