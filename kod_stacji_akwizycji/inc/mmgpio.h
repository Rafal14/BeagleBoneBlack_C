/*!
 * \file   mmgpio.h
 *  
 * Utworzono: 13 lis 2017
 *
 */


#ifndef MMGPIO_HH
#define MMGPIO_HH


/*!
 *  \brief Definicja struktury mmgpio i funkcji do obsługi pinu typu
 *         GPIO
 *
 *  Plik zawiera definicję struktury mmgpio i funkcji zapisu, odczytu
 *  wartości do/z pinu GPIO poprzez operacje na rejestrach procesora
 *  AM335x (bezpośredni dostęp do rejestrów) 
 *
 */




/* obszar adresowy GPIO procesora AM335x - każdy o rozmiarze 4 kB */
#define GPIO0_ADDRESS  0x44E07000
#define GPIO1_ADDRESS  0x4804C000
#define GPIO2_ADDRESS  0x481AC000
#define GPIO3_ADDRESS  0x481AF000
#define GPIO_LENGTH    4096         /* rozmiar przestrzeni adresowej */


#define GPIO_OE           0x134  /* rejestr określający kierunek gpio */
#define GPIO_DATAIN       0x138  /* rejestr wejściowy */
#define GPIO_DATAOUT      0x13C  /* rejestr wyjściowy */
#define GPIO_CLEARDATAOUT 0x190  /* rejestr do wyzerowania wyjścia */
#define GPIO_SETDATAOUT   0x194  /* rejestr do ustawienia wyjścia */


#include <stdint.h>



/*!
 *  \struct mmgpio
 *  \brief Struktura definiująca pin GPIO
 *
 *  \param number    określa numer pinu GPIO
 *  \param contr     wskaźnik na zmapowany obszar pamięci
 *  \param fd        deskryptor do pliku /dev/mem (dostęp do pamięci)
 *
 */
typedef struct mmgpio
{
	int        number;   
	volatile   uint32_t  *contr;
	
	int fd;

} mmgpio;



/*!
 *  \fn create_mm_gpio
 * 
 *  \brief  Mapuje obszara pamięci gpio
 *
 *  \param  gp         wskaźnik na strukturę dla pinów gpio
 *  \param  controller określa numer sterownika GPIO (0...3)
 *  \param  number     określa numer pinu względem sterownika GPIO (0...30)
 *  \retval zwraca wartość 0, gdy obszar pamięci został zmapowany,
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int create_mm_gpio(mmgpio *gp, int controller, int number);



/*!
 *  \fn mm_close
 * 
 *  \brief  Zamyka deskryptor do odwzorowanego obszaru pamięci pamieci 
 *
 *  \param  gp         wskaźnik na strukturę dla pinów gpio
 *
 */
void mm_close(mmgpio *gp);


/*!
 *  \fn mm_set_bit
 * 
 *  \brief  Ustawia stan wysoki na wyjściu gpio
 *
 *  \param  gp         wskaźnik na strukturę dla pinów gpio
 *
 */
void mm_set_bit(mmgpio *gp);


/*!
 *  \fn mm_clear_bit
 * 
 *  \brief  Ustawia stan niski na wyjściu gpio
 *
 *  \param  gp         wskaźnik na strukturę dla pinów gpio
 *
 */
void mm_clear_bit(mmgpio *gp);


/*!
 *  \fn mm_set_output
 * 
 *  \brief  Ustawia kierunek pinu gpio jako wyjście
 *
 *  \param  gp         wskaźnik na strukturę dla pinów gpio
 *
 */
void mm_set_output(mmgpio *gp);


/*!
 *  \fn mm_set_input
 * 
 *  \brief  Ustawia kierunek pinu gpio jako wejście
 *
 *  \param  gp         wskaźnik na strukturę dla pinów gpio
 *
 */
void mm_set_input(mmgpio *gp);


/*!
 *  \fn mm_get_value
 * 
 *  \brief  Pobiera wartość (logiczną) z wejścia typu gpio
 *
 *  \param  gp         wskaźnik na strukturę dla pinów gpio
 *  \retval zwraca wartość logiczną (0 lub 1) jako wartość z wejścia
 *          gpio
 *
 */
uint32_t mm_get_value(mmgpio *gp);






#endif
