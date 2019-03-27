/*!
 * \file   modtcp.h
 * \author Rafał Januszewski
 *  
 * Utworzono: 30 paź 2017
 *
 */

#ifndef MODTCP_H
#define MODTCP_H


#include "buffer.h"

/*!
 * \file
 * \brief Funkcje sprawdzania i parsowania ramki protokołu Modbus TCP/IP
 *        po stronie serwera
 * 
 * Plik zawiera definicję funkcji do sprawdzenia ramki protokołu 
 * Modbus TCP/IP i utworzenia odpowiedzi do przesłanego zapytania 
 * zgodnie ze standardem protokołu Modbus TCP/IP
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
 *  \fn checkModbusFrame
 * 
 *  \brief  Sprawdza poprawność odebranej ramki protokołu Modbus TCP/IP
 *          - poprawność zapytania nadesłanego od klienta
 *
 *  \param  query     wskaźnik na tablicę zawierającą ramkę protokołu
 *  \param  fnc       wskaźnik na wartość funkcji
 *  \param  addr      wskaźnik na wartość adresu
 *  \param  val       wskaźnik na wartość
 *  \retval zwraca wartość 0, gdy nadesłana ramka jest poprawna, 
 *          zwraca -1 w wypadku wystąpienia błędu w ramce
 *
 */
int checkModbusFrame(uint8_t *query, uint8_t *fnc, uint16_t *addr,
                                                   uint16_t *val);


/*!
 *  \fn createModbusResponse
 * 
 *  \brief  Składa ramkę odpowiedzi protokołu Modbus TCP/IP
 *
 *  \param  query     wskaźnik na tablicę zawierającą ramkę zapytania
 *  \param  response  wskaźnik na tablicę zawierającą ramkę odpowiedzi
 *  \param  elem      struktura zawierająca dane pomiarowe
 *  \param  uptime    wartość określająca czas działania urządzenia [s]
 *  \param  prtime    wartość określająca czas działania programu [s]
 *
 */
void createModbusResponse(uint8_t *query, uint8_t *response, 
                        value_t elem, uint32_t uptime, uint32_t prtime);



#endif
