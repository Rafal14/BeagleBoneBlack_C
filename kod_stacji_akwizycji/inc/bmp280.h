/*!
 * \file   bmp280.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 28 paź 2017
 *
 */
 
#ifndef BMP280_H
#define BMP280_H 


#include <unistd.h> 
 
 
#include "i2cdevice.h"
 
 
 
 /*!
 *  \brief Plik zawiera definicję funkcji do obsługi czujnika
 *  ciśnienia i temperatury BMP280
 *
 *  Funkcje zdefiniowane w pliku pozwalają na konfigurację czujnika
 *  i odczyt danych pomiarowych ciśnienia i temperatury
 *
 */
 
 
#define BMP280_ADDR	0x76     /* adres czujnika w magistali I2C */
#define ID_REG 		0xd0     /* adres rejestru zawierającego id */
#define STATUS_REG 	0xf3     /* adres rejestru określającego status */
#define CTRL_REG 	0xf4     /* adres rejestru kontroli pomiaru */
#define CONFIG_REG 	0xf5     /* adres rejestru konfiguracyjnego */
#define RAW_DATA_REG 0xf7    
#define CALIB_REG 	0x88    
#define CALIB_SIZE 	24      



/*!
 *  \fn BMP280_Open
 * 
 *  \brief Uaktywania obługę czujnika BMP280 podłączonego do magistrali 
 *         I2C
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \retval zwraca wartość 0, gdy połączenie z czujniem temperatury 
 *          zostało ustanowione
 *          zwraca -1, gdy nie udało się ustanowić połączenia 
 *          z czujnikiem
 *
 */
int BMP280_Open(i2cdevice *sensor);


/*!
 *  \fn BMP280_Open
 * 
 *  \brief Zamyka połączenie z czujnikiem ciśnienia i temperatury BMP280  
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *
 */
void BMP280_Close(i2cdevice *sensor);

/*!
 *  \fn BMP280_Config
 * 
 *  \brief Konfiguruje połącznie z czujnikiem BMP280 (określa adres
 *         urządzenia)
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *
 */
void BMP280_Config(i2cdevice *sensor);

/*!
 *  \fn BMP280_Read_Id
 * 
 *  \brief  Odczytuje wartość id czujnika BMP280
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \param  buf wskaźnik na bufor czytający
 *  \retval zwraca wartość 0, gdy odczytano wartość id czujnika BMP280
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_Read_Id(i2cdevice *sensor, char *buf);


/*!
 *  \fn BMP280_ReadCtrMeas
 * 
 *  \brief  Odczytuje zawartość rejestru sterowania pomiarem 
 *          czujnika BMP280
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \param  buf wskaźnik na bufor czytający
 *  \retval zwraca wartość 0, gdy odczytano zawartość rejestru 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_ReadCtrMeas(i2cdevice *sensor, char *buf);


/*!
 *  \fn BMP280_WriteCtrMeas
 * 
 *  \brief  Zapisuje dane do rejestru kontroli pomiaru
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \retval zwraca  wartość 0, gdy zapisano dane do rejestru slave'a, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_WriteCtrMeas(i2cdevice *sensor);


/*!
 *  \fn BMP280_ReadConfigReg
 * 
 *  \brief  Odczytuje zawartość rejestru konfiguracyjnego
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \param  buf wskaźnik na bufor czytający
 *  \retval zwraca wartość 0, gdy odczytano zawartość rejestru 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_ReadConfigReg(i2cdevice *sensor, char *buf);


/*!
 *  \fn BMP280_WriteConfigReg
 * 
 *  \brief  Zapisuje dane do rejestru konfiguracyjnego
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \retval zwraca  wartość 0, gdy zapisano dane do rejestru slave'a, 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_WriteConfigReg(i2cdevice *sensor);



/*!
 *  \fn BMP280_ReadCalib
 * 
 *  \brief  Odczytuje zawartość rejestrów zawierających współczynniki 
 *          pomiaru temperatury i ciśnienia
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \param  buf wskaźnik na bufor czytający
 *  \retval zwraca wartość 0, gdy odczytano zawartość rejestrów 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_ReadCalib(i2cdevice *sensor, char *buf, int bufsize);


/*!
 *  \fn BMP280_ReadRawData
 * 
 *  \brief  Odczytuje zawartość rejestrów zawierających nieprzetworzone
 *          dane pomiarowe temperatury i ciśnienia
 *
 *  \param  sensor wskaźnik na strukturę urządzenia podłączonego od I2C 
 *  \param  buf wskaźnik na bufor czytający
 *  \retval zwraca wartość 0, gdy odczytano zawartość rejestrów 
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_ReadRawData(i2cdevice *sensor, char *buf, int bufsize);


/*!
 *  \fn BMP280_Measure
 * 
 *  \brief  Oblicza wartość temperatury i ciśnienia na podstawie
 *          pobranych danych z rejestrów czujnika
 *
 *  \param[in]   sensor wskaźnik na strukturę urządzenia podłączonego 
 *               od I2C 
 *  \param[out]  temperature wskaźnik na wartość temperatury
 *  \param[out]  pressure wskaźnik na wartość ciśnienia
 *  \retval zwraca wartość 0, gdy odczytano zawartość rejestrów 
 *          pomiarowych i obliczono wartości ciśnienia i temperatury
 *          zwraca -1 w przeciwnym wypadku
 *
 */
int BMP280_Measure(i2cdevice *sensor, long *temperature,
                   unsigned long *pressure);



#endif
