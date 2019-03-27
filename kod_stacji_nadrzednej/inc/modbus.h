/*!
 * \file   modbus.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 30 paź 2017
 *
 */


#ifndef MODBUS_H
#define MODBUS_H


/*!
 * \file
 * \brief Funkcje tworzenia zapytania protokołu Modbus TCP/IP
 *        i dekodowania ramki odpowiedzi odebranej od serwera
 * 
 * Plik zawiera definicję funkcji tworzenia zapytania protokołu 
 * Modbus TCP/IP i dekodowania ramki odpowiedzi odebranej od serwera
 * wraz ze sprawdzenie jej poprawności
 * 
 */



/* funkcje protokołu Modbus */
#define READ_COILS 0x01               /* odczytaj wyjścia cyfrowe*/

#define READ_DISCRETE_INPUTS 0x02   /* odczytaj wejścia cyfrowe */


#define READ_HOLDING_REGISTERS 0x03 /* odczytaj wartość rejestów trzym */


#define READ_INPUT_REGISTER 0x04  /* odczytaj rejestry wejściowe -
                                     wartosci analogowe */

#define WRITE_SINGLE_COIL 0x05   /* wymuś wartość na wyjściu cyfrowym */

#define WRITE_SINGLE_REGISTER 0x06  /* ustaw wartość pojedynczego rejestru */

#define WRITE_MULTIPLE_COILS 0x0f   /* wymuś stan na wielu wyjściach cyfrowych */

#define WRITE_MULTIPLE_REGISTERS 0x10   /* ustaw wartości wielu rejestrów */

#define RDWR_MULTIPLE_REGISTERS 0x17    /* odczytaj i zapisz wiele rej */


#define RD_DEVICE_STATUS 0x41       /* odczytaj status urządzenie */

#define NONSTOP_MEASURE 0x45        /* realizuj pomiar ciągły */




/*!
 *  \fn createModbusRequest
 * 
 *  \brief  Składa ramkę zapytania protokołu Modbus TCP/IP
 *
 *  \param  buf       wskaźnik na tablicę zawierającą ramkę zapytania
 *  \param  tranId    numer transakcji
 *  \param  funCode   kod funcji 
 *  \param  startAddr adres początkowy obszaru pamięci
 *  \param  numReg    liczba rejestrów do odczytu
 *  \param  numRegWr  liczba rejestrów do zapisu
 *
 */
void createModbusRequest(uint8_t *buf, uint16_t *tranId, 
                         uint8_t funCode, uint16_t startAddr, 
                         uint16_t numReg, uint16_t numRegWr); 


/*!
 *  \fn decodeModbusFrame
 * 
 *  \brief  Sprawdza poprawność odebranej ramki protokołu Modbus TCP/IP
 *          - poprawność odpowiedzi otrzymanej od serwera
 *
 *  \param  buf       wskaźnik na tablicę zawierającą ramkę protokołu
 *  \param  dataAddr  określa początek adresu rejestru
 *  \retval zwraca wartość 0, gdy nadesłana ramka jest poprawna, 
 *          zwraca -1 w wypadku wystąpienia błędu w ramce
 *
 */
int decodeModbusFrame(uint8_t *buf, uint16_t dataAddr);


#endif
