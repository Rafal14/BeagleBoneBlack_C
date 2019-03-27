/*!
 * \file   i2cdevice.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 28 paź 2017
 *
 */
 
#ifndef I2CDEVICE_H
#define I2CDEVICE_H


/*!
 *  \brief Definicja struktury i2cdevice i funkcji do obsługi urządzeń
 *        podłączonych do magistrali I2C
 *
 *  Plik zawiera definicję struktury i2cdevice, która specyfikuje 
 *  urządzenie podłączone do magistrali I2C
 *
 */



#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <stdio.h>



/*!
 *  \struct i2cdevice
 *  \brief Struktura definiująca urządzenie podłączone do magistrali I2C
 *
 *  \param fd        deskryptor do pliku /dev/i2c-*
 *  \param busNo    określa number magistrali do którego podłączono
 *                  urządzenie 
 *  \param addrDev   określa adres urządzenia
 *
 */
typedef struct i2cdevice
{
	int           fd;
	int           busNo;
	int8_t        addrDev;
	
} i2cdevice;


/*!
 *  \fn enableI2C
 * 
 *  \brief Uaktywania obługę określonej magistrali I2C
 *
 *  \param  busNumber określa numer magistrali I2C
 *  \retval zwraca wartość 0, gdy magistrala obsługa I2C została
 *          włączona
 *          zwraca -1, gdy aktywacja magistrali I2C się nie powiodła
 *
 */
int enableI2C(int busNumber);


/*!
 *  \fn i2cOpen
 * 
 *  \brief  Uaktywania połącznie z określą magistralą I2C
 *
 *  \param  dev       wskaźnik na strukturę i2cdevice
 *  \param  busNumber określa numer magistrali I2C
 *  \retval zwraca wartość 0, gdy możliwy jest dostęp do magistrali I2C, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int i2cOpen(i2cdevice *dev, int busNumber);


/*!
 *  \fn i2cOpen
 * 
 *  \brief  Zamyka połącznie z określą magistralą I2C
 *
 *  \param  dev       wskaźnik na strukturę i2cdevice
 *
 */
void i2cClose(i2cdevice *dev);



/*!
 *  \fn i2cConfig
 * 
 *  \brief  Konfiguruje połączenie z urządzeniem poprzez magistralę
 *          I2C, określa adres slave'a
 *
 *  \param  dev  wskaźnik na strukturę i2cdevice
 *  \param  addr określa numer magistrali I2C
 *  \retval zwraca wartość 0, gdy możliwy jest dostęp do magistrali I2C, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int i2cConfig(i2cdevice *dev, int8_t addr);


/*!
 *  \fn i2cReadData
 * 
 *  \brief  Odczytuje dane ze slave'a podłączonego do magistrali I2C
 *
 *  \param  dev     wskaźnik na strukturę i2cdevice
 *  \param  regAddr adres rejestru który ma zostać odczytany
 *  \param  readBuf wskaźnik na bufor do odczytu
    \param  bufSize rozmiar bufor do odczytu
 *  \retval zwraca  wartość 0, gdy odczytano dane z rejestru slave'a, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int i2cReadData(i2cdevice *dev, unsigned char regAddr, char *readBuf,
                int bufSize);


/*!
 *  \fn i2cWriteData
 * 
 *  \brief  Zapisuje dane do określonego rejestru slave'a
 *
 *  \param  dev          wskaźnik na strukturę i2cdevice
 *  \param  writeBuf     wskaźnik na bufor do zapisu danych
    \param  writeBufSize rozmiar bufor do zapisu
 *  \retval zwraca  wartość 0, gdy zapisano dane do rejstru slave, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int i2cWriteData(i2cdevice *dev, char *writeBuf, int writeBufSize);





#endif
