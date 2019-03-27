/* kod dla klienta w komunikacji tcp/ip */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>
#include <signal.h>



/* port lokalny */
#define PORT 502

/* długość ramki protokołu Modbus TCP/IP */
#define BUFSIZE 60


#include "modbus.h"



volatile int interrflag = 0;



void signalHandler(int signalno)
{
	printf("Przerwano pomiar ciagly\n\n");
	interrflag = 1;
}



int main(void)
{
	/* deskryptor gniazda */
	int socket_fd;
	
	int retval;
	
	
	int option, opt;
	
	/* struktury zawierające adresy gniazd */
	struct sockaddr_in server_addr;
	
	struct hostent *hostserv;
	

	/* ramka protokołu Modbus TCP/IP */
	uint8_t frame[BUFSIZE];
	
	
	/* ramka dla trybu ciągłego */
	uint8_t resframe[BUFSIZE];
	
	
	/* numer transakcji */
	uint16_t tran =1;
	
	/* wypełenie sygnału PWM */
	int duty;
	
	uint16_t dAddress; /* adres początkowy czytanego resjestru */
	
	
	/* załączenie przechwtywania sygnału SIGINT */
	signal(SIGINT, signalHandler);
	

	
	/* tworzenie gniazda 
	 * 
	 * AF_INET     - domena poł. internetowego IPv4
	 * SOCK_STREAM - strumieniowy przesył danych
	 * */
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
	
	if (socket_fd<0) {
		perror("Nie mozna utworzyc gniazda");
		exit(1);
	}
	
		
	/* nadanie adresu */
	server_addr.sin_family      = AF_INET;
	

	hostserv = gethostbyname("192.168.7.2");
	
	memcpy(&server_addr.sin_addr, hostserv->h_addr, hostserv->h_length);
	
	server_addr.sin_port        = htons(PORT);
	
	/* polaczenie */
	if ( connect(socket_fd, (struct sockaddr *) &server_addr, 
	                                       sizeof(server_addr))<0) {
		perror("blad poloczenia");
		exit(1);
	}
	printf("polaczenie nawiazane\n");
	
	
	
	do {
		printf("1. Odczyt danych\n");
		printf("2. Zapis danych\n");
		printf("0. Zakoncz\n");
		printf("Wybierz opcje:\t");
		scanf("%d", &option);
		printf("\n");
		
		assert(option < 3);
		
		
		
		if ( option == 1 ) {
			do {
				printf("1.  Odczytaj stan wyjsc cyfrowych\n");
				printf("2.  Odczytaj stan wejsc cyfrowych\n");
				printf("3.  Odczytaj stan wejsc analogowych\n");
				printf("4.  Odczytaj wypelnienie sygnalu PWM\n");
				printf("5.  Odczytaj wartosc temperatury i cisnienia z czujnika BMP280\n");
				printf("6.  Odczytaj wartosci przyspieszen z akcelerometru AXDL345\n");
				printf("7.  Odczytaj wartosci wilgotnosci i temperatury z czujnika DHT11\n");
				printf("8.  Odczytaj status\n");
				printf("9.  Ciagly odczyt wartosci pomiarowych\n");
				printf("0.  Powrot\n");
				printf("Wybierz opcje:\t");
				scanf("%d", &opt);
				printf("\n");
				
				assert(opt < 10);
				
				switch(opt) {
					case 0: break;
					
					/* Odczyt stanu wyjscia LED1 */
					case 1:
						createModbusRequest(frame, &tran, 
						                    READ_COILS, 0, 1, 
						                    0);
						break;
					
					/* Odczyt stanu wejsc SW1 i SW2 */
					case 2:
						createModbusRequest(frame, &tran, 
						                    READ_DISCRETE_INPUTS, 8, 2, 
						                    0);
						break;
					
					/* Odczyt wartosci analogowych z potencjometru,
					 * fotorezystora oraz z czujnika temperatury  */
					case 3:
						createModbusRequest(frame, &tran, 
						                    READ_INPUT_REGISTER,16,3, 
						                    0);
						break;
					
					/* Odczyt wypelnienia sygnalu PWM */		
					case 4:
						createModbusRequest(frame, &tran, 
						                    READ_HOLDING_REGISTERS,48,2, 
						                    0);
						dAddress = 48;
						break;
						
					/* Odczyt wartosci temperatury i cisnienia 
					 * z czujnika BMP280 */		
					case 5:
						createModbusRequest(frame, &tran, 
						                    READ_HOLDING_REGISTERS,52,4, 
						                    0);
						dAddress = 52;
						break;
					
					/* Odczytaj wartości przyspieszeń */	
					case 6:
						createModbusRequest(frame, &tran, 
						                    READ_HOLDING_REGISTERS,60,6, 
						                    0);
						dAddress = 60;
						break;
						
					/* Odczytaj wartości wilgotności i temperatury z DHT11 */	
					case 7:
						createModbusRequest(frame, &tran, 
						                    READ_HOLDING_REGISTERS,80,4, 
						                    0);
						dAddress = 80;
						break;
					
					/* Odczyt statusu */	
					case 8:
						createModbusRequest(frame, &tran, 
						                    RD_DEVICE_STATUS, 0, 0, 0);
						dAddress = 0;
						break;
						
						
					/* Realizuj ciągły odczyt wartości pomiarowych */	
					case 9:
						createModbusRequest(frame, &tran, 
						                    NONSTOP_MEASURE, 0, 0, 0);
						dAddress = 0;
						break;
						
					default:
						printf("Nieprawidlowa opcja\n");
						break;
				}
				
				
				if ( opt !=0 ) {
					
					
					/* sprawdzenie czy wybrano tryb ciągły pomiaru */
					if ( opt == 9 ) {
						
						do {
							
							retval = send(socket_fd, frame, BUFSIZE, 0);
							
							if (retval<0) {
								perror("Nie mozna wyslac komunikatu");
							}
							
							retval = recv(socket_fd, resframe, BUFSIZE, 
							                                         0);
							                                         
							decodeModbusFrame(resframe, dAddress);                                         
							sleep(3);
							
						}while(interrflag!=1);
						interrflag = 0;
					}
					/* sprawdź czy wybrano opcję "na żądanie" */
					else {
					
						retval = send(socket_fd, frame, BUFSIZE, 0);
				
					if (retval<0) {
						perror("Nie mozna wyslac komunikatu");
						exit(1);
					}
					
					retval = recv(socket_fd, frame, BUFSIZE, 0);

				
					if (retval < 0 ) { perror("blad odczytu"); break; }
								
					
					/* dekodowanie otrzymanej ramki Modbus TCP/IP */ 
					decodeModbusFrame(frame, dAddress);	
				
					if (retval == 0) {
						printf("Polaczenie zakonczone"); break;
					}
					
				}
			}			
				
			}while(opt!=0);
		}
		
		
		if ( option == 2 ) {
			do {
				printf("1. Zapal LED1\n");
				printf("2. Zgas LED1\n");
				printf("3. Zmien wypelnienie sygnalu PWM\n");
				printf("0. Powrot\n");
				printf("Wybierz opcje:\t");
				scanf("%d", &opt);
				printf("\n");
				
				
				assert(opt < 4);
				
				
				switch(opt) {
					case 0: break;
					
					/* Zapalenie LED1 */
					case 1:
						createModbusRequest(frame, &tran, 
						                    WRITE_SINGLE_COIL,0, 0xff00, 
						                    0);
						break;
					
					/* Zgaszenie LED1 */
					case 2:
						createModbusRequest(frame, &tran, 
						                    WRITE_SINGLE_COIL,0, 0x0000, 
						                    0);
						break;
					
					/* Zmiana wypełnienia PWM  */
					case 3:
						printf("Podaj wartosc wypelnienia %%: ");
						scanf("%d", &duty);
						assert(duty < 101);
						createModbusRequest(frame, &tran, 
						                    WRITE_SINGLE_REGISTER, 48,
						                    duty, 0);
						break;
					
					default:
						printf("Nieprawidlowa opcja\n");
						break;
				}
				
				if (opt != 0) {
					
					retval = send(socket_fd, frame, BUFSIZE, 0);
				
					if (retval<0) {
						perror("Nie mozna wyslac komunikatu");
						exit(1);
					}
				
					retval = recv(socket_fd, frame, BUFSIZE, 0);
				
				
					if (retval < 0 ) { perror("Blad odczytu"); exit(-1); }
					
			
					/* dekodowanie otrzymanej ramki Modbus TCP/IP */ 
					decodeModbusFrame(frame, dAddress);
									
					
				
					if (retval == 0) {
						printf("Polaczenie zakonczone"); exit(-1);
					}
				}				
				
			}while(opt!=0);
		}
		
	}while(option!=0);
	
	printf("Koniec dzialania programu\n");
	
	
	close(socket_fd);      /* zamknięcie gniazda */
		
		
		
	return 0;
}

