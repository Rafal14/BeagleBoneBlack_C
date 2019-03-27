/*!
 * \file   spidevice.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 28 paź 2017
 *
 */
 
#ifndef SPIDEVICE_H
#define SPIDEVICE_H


/*!
 *  \brief Definicja struktury spidevice i funkcji do obsługi urządzeń
 *        podłączonych do magistrali SPI
 *
 *  Plik zawiera definicję struktury spidevice, która specyfikuje 
 *  urządzenie podłączone do magistrali SPI
 *
 */


#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>



/*!
 *  \struct spidevice
 *  \brief Struktura definiująca urządzenie podłączone do magistrali SPI
 *
 *  \param fd        deskryptor do pliku /dev/spidev*
 *  \param busNo     określa number magistrali do którego podłączono
 *                   urządzenie
 *  \param csNo      określa numer pinu Chip Select (CS)  
 *
 */
typedef struct spidevice
{
	int   fd;
	int   busNo;
	int   csNo;    
	
	
	uint32_t   speed;
	uint8_t   usec_delay;
	uint8_t   bits_per_word;
	
} spidevice;


/*!
 *  \fn enableSPI
 * 
 *  \brief Uaktywania obługę określonej magistrali SPI
 *
 *  \param  busNumber określa numer magistrali SPI
 *  \retval zwraca wartość 0, gdy magistrala obsługa SPI została
 *          włączona
 *          zwraca -1, gdy aktywacja magistrali SPI się nie powiodła
 *
 */
int enableSPI(int busNumber);


/*!
 *  \fn spiOpen
 * 
 *  \brief  Uaktywania połącznie z określą magistralą SPI
 *
 *  \param  dev       wskaźnik na strukturę spidevice
 *  \param  busNumber określa numer magistrali SPI
 *  \param  csNumber  określa numer pinu Chip Select
 *  \retval zwraca wartość deskryptor do magistrali SPI, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int spiOpen(spidevice *dev, int busNumber, int csNumber);


/*!
 *  \fn spiOpen
 * 
 *  \brief  Zamyka połącznie z określą magistralą SPI
 *
 *  \param  dev       wskaźnik na strukturę spidevice
 *
 */
void spiClose(spidevice *dev);



/*!
 *  \fn spiConfig
 * 
 *  \brief  Konfiguruje połączenie z urządzeniem poprzez magistralę
 *          SPI, określa adres slave'a
 *
 *  \param  dev  wskaźnik na strukturę spidevice
 *  \param  addr określa numer magistrali SPI
 *  \retval zwraca wartość 0, gdy możliwy jest dostęp do magistrali SPI, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int spiConfig(spidevice *dev, uint8_t mode, uint8_t bitNum,
              uint32_t speed);


/*!
 *  \fn spiReadData
 * 
 *  \brief  Odczytuje dane z określonego rejestru urządzenia 
 *          podłączonego do SPI
 *
 *  \param  dev     wskaźnik na strukturę spidevice
 *  \param  addrReg adres rejestru do odczytu
 *  \param  readBuf wskaźnik na bufor do odczytu
    \param  size rozmiar bufor do odczytu
 *  \retval zwraca  wartość 0, gdy odczytano dane z rejestru slave'a, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int spiReadData(spidevice *dev,  unsigned char  addrReg,
                                 unsigned char *readBuf, int size);
                

/*!
 *  \fn spiWriteData
 * 
 *  \brief  Zapisuje dane do określonego rejestru urządzenia 
 *          podłączonego do SPI
 *
 *  \param  dev     wskaźnik na strukturę spidevice
 *  \param  sendBuf wskaźnik na bufor do zapisu
 *  \param  readBuf wskaźnik na bufor do odczytu
    \param  size    rozmiar bufor do zapisu/odczytu
 *  \retval zwraca  wartość 0, gdy zapisano dane do rejestru slave'a, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */                
int spiWriteData(spidevice *dev, unsigned char *sendBuf, 
                 unsigned char *readBuf, int size);
                



#endif
