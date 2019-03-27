/*!
 * \file   ADXL345.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 28 paź 2017
 *
 */
 
#ifndef ADXL345_H
#define ADXL345_H 


#include <unistd.h> 
#include <string.h>
#include <stdio.h>
 
 
#include "spidevice.h"
 
 
 
 /*!
 *  \brief Plik zawiera definicję funkcji do obsługi akcelerometru
 *         ADXL345
 *
 *  Funkcje zdefiniowane w pliku pozwalają na konfigurację akcelerometru
 *  i odczyt przyspieszenia w trzech osiach
 *
 */
 
 
#define ID_REGISTER   	0x00     /* adres rejestru zawierającego id */
#define POWER_CTL       0x2D
#define DATA_FORMAT     0x31
#define DATA_START      0X32
    



/*!
 *  \fn ADXL345_Open
 * 
 *  \brief Uaktywania obługę czujnika ADXL345 podłączonego do magistrali 
 *         SPI
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od SPI 
 *  \retval zwraca wartość 0, gdy połączenie z czujniem temperatury 
 *          zostało ustanowione
 *          zwraca -1, gdy nie udało się ustanowić połączenia 
 *          z czujnikiem
 *
 */
int ADXL345_Open(spidevice *sensor);


/*!
 *  \fn ADXL345_Open
 * 
 *  \brief Zamyka połączenie z czujnikiem ciśnienia i temperatury ADXL345  
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od SPI 
 *
 */
void ADXL345_Close(spidevice *sensor);

/*!
 *  \fn ADXL345_Config
 * 
 *  \brief Konfiguruje połącznie z czujnikiem ADXL345 (określa adres
 *         urządzenia)
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od SPI 
 *
 */
void ADXL345_Config(spidevice *sensor);

/*!
 *  \fn ADXL345_Read_Id
 * 
 *  \brief  Odczytuje wartość id czujnika ADXL345
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od SPI 
 *  \param  buf wskaźnik na bufor czytający
 *  \retval zwraca wartość 0, gdy odczytano wartość id czujnika ADXL345
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int ADXL345_Read_Id(spidevice *sensor);


/*!
 *  \fn ADXL345_Enable_Measurement
 * 
 *  \brief  Aktywuje tryb pomiaru przyspieszenia
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od SPI 
 *  \retval zwraca wartość 0, gdy aktywowano tryb pomiaru
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int ADXL345_Enable_Measurement(spidevice *sensor);


/*!
 *  \fn ADXL345_SetFullResolution
 * 
 *  \brief  Odczytuje wartość id czujnika ADXL345
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od SPI 
 *  \retval zwraca wartość 0, gdy ustawiono 13-bitowej rozdzielczości
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int ADXL345_SetFullResolution(spidevice *sensor);


/*!
 *  \fn ADXL345_ReadData_XYZ
 * 
 *  \brief  Odczytuje zmierzone przyspieszenia względem osi X,Y,Z 
 *          [m/s^2]
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od SPI 
 *  \param  x wskaźnik wartość przyspieszenia względem osi X
 *  \param  y wskaźnik wartość przyspieszenia względem osi Y
 *  \param  z wskaźnik wartość przyspieszenia względem osi Z
 *  \retval zwraca wartość 0, gdy odczytano wartość przyspieszenia
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int ADXL345_ReadData_XYZ(spidevice *sensor, double *x, double *y, 
                                                       double *z);



#endif
