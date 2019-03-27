/*!
 *  \file main.c
 *
 *  Utworzno: 30 paź 2017
 *     Autor: Rafał Januszewski
 */


/*
 * Program do wymiany informacji za pomocą gniazdek
 * TCP
 *
 * Odbieraja komunikaty od klienta i wysyła odpowiedź
 *
 *
 */



#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <math.h>


#include <sys/time.h>
#include <netinet/tcp.h>

#include <signal.h>


#include "gpio.h"
#include "adc.h"
#include "pwm.h"
#include "i2cdevice.h"
#include "bmp280.h"
#include "buffer.h"
#include "modtcp.h"
#include "spidevice.h"
#include "adxl345.h"
#include "mmgpio.h"
#include "dht11.h"
#include "timedht.h"




#define PORT 502     /* MODBUS TCP IP PORT */

#define MSGSIZE 60   /* rozmiar ramki protokołu Modbus TCP/IP */


#define CONFIG_BUFSIZE 10   




volatile int interrflag = 0;


void signalHandler(int signalno)
{
	printf("Przerwano pomiar ciagly\n\n");
	interrflag = 1;
}




int main(void) {
	
	int fd, sizemem, retval;
	
	/* deskryptory gniazd: serwera i klienta */
	int socket_fd, clisocket_fd, len, stat;
	
	/* struktury zawierające adresy gniazd */
	struct sockaddr_in server_addr;
	
	/* ramka przesyłana między klientem a serwerem */
	uint8_t frame[MSGSIZE];
	
	
	/* ramka przesyłana między klientem a serwerem */
	uint8_t resp[MSGSIZE];
	
	/* wskaźnik na bufor cykliczny */
	buffer_t  *cirBuf;
	pid_t      pid;
	

	
	struct timeval t1, t2;  
	int    fproc;
	char   prBuf[20];
	
	char  *ptime;
	
	uint32_t   devtime;
	uint32_t   progTime;
	
	
	int opt=0;
	
	/* pin GPIO1_17 - pin data DHT11 */
	int gp_contr = 1;
	int gp_number = 17;
	
	float  dhtTemp, dhtHum;    /* wartości pomiarowe z czujnika DHT11 */
	mmgpio dhtData;            /* pin gpio1_17 */
	
	/* pobierz wartosc czasu */
	gettimeofday(&t1, NULL);

	
	
	/* definicje zmiennych odnoszących się do pomiarów */
	long          temp;   /* temperatura z czujnika cyfrowego BMP280 */          
	unsigned long pres;   /* ciśnienie z czujnika cyfrowego BMP280 */ 
	
	/* pomiary przyspieszeń */
	double       accelX, accelY, accelZ;
	
	unsigned int status;
	
	float         duty; 
	
	/* obiekt pomiaru */	 
	value_t       elem;
	
	FILE         *ptrFile;
	const char*   configFilename = "configfile"; 
	char          configBuf[CONFIG_BUFSIZE];
	
	int           configTime, timecnt;

	
	/* określa numer sterownika przetwornika ADC */
	int adcDriver;
	
	/* wartości odczytane z wejść 0, 1, 6 przetwornika ADC*/
	int ain0, ain1, ain6;
	
	double lain;

	/* struktura dla czujnika ciśnienia podłączonego do I2C1 */
	i2cdevice bmp280;
	
	/* struktura dla akcelerometru */
	spidevice adxl345;
	
	/* struktury dla diody RGB sterowanej sygnałem PWM */
	pwm       pwmGreen;
	pwm       pwmBlue;
	pwm       pwmRed;

	/* pin typu gpio - LED czerwona */
	gpio      ledRed, sw1, sw2;
	
	
	
	/* Wczytanie konfiguracji odczytu danych pomiarowych */
	ptrFile = fopen(configFilename, "r");
	
	if ( ptrFile == NULL ) {
		fprintf(stderr, "Blad. Nie mozna otworzyc pliku z konfiguracja\n");
		exit(-1);
	}
	
	if (fread(configBuf, sizeof(char), CONFIG_BUFSIZE, ptrFile) == 0) {
		fprintf(stderr, "Blad. Nie mozna odczytac danych z pliku\n");
		fclose(ptrFile);
		exit(-1);
	}
	fclose(ptrFile);
	
	configTime = atoi(configBuf);
	
	if ( configTime < 1 ) {
		fprintf(stderr, "Blad. Wartosc czasu odczytu jest niepoprawna\n");
		exit(-1);
	}
	
	
	
	/* Konfiguracja */
	
	/* konfiguracja pinu gpio jako wyjście- LED */   
	gpioEnable(&ledRed, 60, 0);
	gpioSetDirection(&ledRed, "out");
	
	gpioEnable(&sw1, 47, 0);
	gpioSetDirection(&sw1, "in");
	
	gpioEnable(&sw2, 26, 0);
	gpioSetDirection(&sw2, "in");                    

	/* konfiguracja przetwornika ADC */               
	enableAdc();
	adcDriver = adcCheckDriverNo();
	                                      
	/* konfiguracja magistrali SPI1 */                                      
	enableSPI(1);                                      
	                                                  
	/* konfiguracja magistrali I2C1 */                
	enableI2C(1);                                     
	
	/* konfiguracja czujnika ciśnienia BMP280 */                
	BMP280_Open(&bmp280);                           
	BMP280_Config(&bmp280);                           
	
	/* konfiguracja pomiaru ciśnienia */             
	BMP280_WriteCtrMeas(&bmp280);  
	BMP280_WriteConfigReg(&bmp280);   
	
	/* konfiguracja akcelerometru ADXL345 */
	ADXL345_Open(&adxl345);
	ADXL345_Config(&adxl345);   
	ADXL345_Enable_Measurement(&adxl345);               
                                    
	
	/* załączenie pinów PWM */                        
	enablePwmPin(&pwmRed, 8, 13);
	enablePwmPin(&pwmGreen, 9, 14);
	enablePwmPin(&pwmBlue, 9, 21);                    

	/* sprawdzenie sterowaników poszczególnych pinów PWM */     
	pwmCheckDriverNo(&pwmGreen);                             
	pwmCheckDriverNo(&pwmBlue);
	pwmCheckDriverNo(&pwmRed);                                  

	/* załączenie obsługi PWM w uC */                           
	enablePwmControl();                                        

	/* określenie polaryzacji sygnału PWM - wyzwalanie sygnału od "0" */
	pwmSetPolarity(&pwmGreen, 0);
	pwmSetPolarity(&pwmBlue, 0);
	pwmSetPolarity(&pwmRed, 0);                                 
	
	/* ustawienie okresu sygnału PWM  */                                                            
	pwmSetPeriod(&pwmBlue,  200000);
	pwmSetPeriod(&pwmGreen, 200000);
	pwmSetPeriod(&pwmRed,   200000);                           

	/* załączenie sygnałów PWM */ 
	pwmRun(&pwmGreen, 1);
	pwmRun(&pwmBlue, 1);
	pwmRun(&pwmRed, 1);


	/* mapowanie obszaru pamięci dla mmgpio */
	create_mm_gpio(&dhtData, gp_contr, gp_number);

	/* opóźnienie czasowe dla inicjalizacji czujnika
           DHT11 */
	sleep(1);

		

	/* usunięcie obszaru pamięci współdzielonej */
	shm_unlink("cirBuffer");
	
	/* utworzenie obszaru pamięci współdzielonej */
	fd = shm_open("cirBuffer", O_RDWR|O_CREAT, 0774);
	
	if (fd == -1) {
		perror("shm_open");
		exit(-1);
	}
	
	/* ustawienie rozmiaru pamięci współdzielonej */
	sizemem = ftruncate(fd, BUFSIZE);
	
	if (sizemem < 0) {
		perror("ftruncate");
		exit(-1);
	}
	
	/* odwzorowanie segmentu wskazywanego przez deskryptor w pamięci
	 * procesów */
	 cirBuf = (buffer_t *) mmap(0, BUFSIZE, PROT_READ|PROT_WRITE,
	                            MAP_SHARED, fd, 0);
	                            
	 if (cirBuf == NULL) {
		 perror("mmap error");
		 exit(-1);
	 }
	 
	 /* inicjalizacja semafora nienazwanego do blokowania dostępu do 
	  * pamięci współdzielonej */
	 retval = sem_init(&(cirBuf->mutex), 1, 1);
	 
	 if (retval < 0) {
		 perror("sem_init");
		 exit(-1);
	 }
	 
	 /* inicjalizacja bufora cyklicznego */
	 initBuf(cirBuf);
	 
	 /* utwórz nowy proces funkcją fork() */
	 pid = fork();
	 
	 /* proces potomny - obsługa komunikacji TCP/IP */
	 if (pid == 0) {
		 
		 /* kod funkcji */
		 uint8_t  fcode;
		 uint16_t addrReg, valReg;
		 
		 value_t el;
		 
		 float percentDuty;


		 /* komunikacja tcp/ip   */
		 
		 /* Tworzenie gniazda 
		 * 
		 * AF_INET     - domena poł. internetowego IPv4
		 * SOCK_STREAM - strumieniowy przesył danych
		 */
		 socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		 
		 if ( socket_fd<0 ) {
			 perror("Nie mozna utworzyc gniazda");
			 exit(1);
		 }
 
		 
		 
		 /* nadanie adresu */
		 server_addr.sin_family      = AF_INET;
		 server_addr.sin_addr.s_addr = INADDR_ANY;
		 server_addr.sin_port        = htons(PORT);
		 
		 
		 opt=1;
		 setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, 
		                                                  sizeof(int));
		 
		 
		  /* przypisanie adresu do gniazda */
		 if ( bind(socket_fd,(struct sockaddr *) &server_addr, 
		          sizeof(server_addr)) < 0 ) {
			 perror("Nie mozna przypisac adresu do gniazda");
			 exit(1);
		 }
	 
		 
		 /* test - sprawdzenie danych o połączeniu */
		 len = sizeof(server_addr);
		 if ( getsockname(socket_fd,(struct sockaddr *) &server_addr, 
		        (socklen_t *) &len ) < 0 ) {
			 perror("Nie mozna uzyskac danych o polaczeniu");
			 exit(1);
		 }
		 

		 printf("Numer portu %d\n", ntohs(server_addr.sin_port));
		 
		 /* rozpocznij przyjmowanie połączeń */
		 listen(socket_fd, 5); 
		 
		
		 /* pobiera żądanie i tworzy uchwyt do nowego podłączonego 
		  * gniazda */ 
		 clisocket_fd = accept(socket_fd, 0, 0);
			 
		 /* sprawdzenie poprawności kom */
		 if (clisocket_fd<0) {
			 perror("Nie mozna zestawic polaczenia");
			 exit(1);
		 }
			 
		 opt=1;
		 if (setsockopt(clisocket_fd, IPPROTO_TCP, TCP_NODELAY, &opt, 
		                                       sizeof(int))) {
			 perror ("setsockopt()");
			 exit (1);
		 }
		 
		 printf("Polaczenie nawiazane - %d\n", socket_fd);
			 
		 do {
			 retval = recv(clisocket_fd, frame, MSGSIZE, 0);              
				           
			 if ( retval<0) {
				 perror("Nie odebrano danych");
				 exit(1);
			 }
			 
			 if (retval == 0 ) {
				 printf("Zakonczono polaczenie z klientem\n");
				 break;
			 }

				 
			 if ( checkModbusFrame(frame, &fcode, &addrReg, 
			                                          &valReg) == 0 ) {
				 
				 switch(fcode) {
					 
					 case WRITE_SINGLE_COIL: 
					 /* zapalenie/wyłączenie diody LED1 */
						if (addrReg == 0) {
							if (valReg == 0xff00) {
								gpioSetValue(&ledRed, 1);
							}
							else if (valReg == 0x0000) {
								gpioSetValue(&ledRed, 0);
							}
							else {
								printf("Nie zapisano wartosci\n");
							}
						}
						break;
						
					 case WRITE_SINGLE_REGISTER:
					 /*zmiana wypełenia sygnalu PWM */
						 if (addrReg == 48) {
							 percentDuty     = valReg / 100.0;
							 pwmSetDutyCycle(&pwmRed,   percentDuty);
							 pwmSetDutyCycle(&pwmGreen, percentDuty);
							 pwmSetDutyCycle(&pwmBlue,  percentDuty);
						 }
						 break;
					 
					 case READ_COILS:
					 case READ_DISCRETE_INPUTS:
					 case READ_INPUT_REGISTER:
					 case READ_HOLDING_REGISTERS:
					 case NONSTOP_MEASURE:
						
						sem_wait(&(cirBuf->mutex));
						readElem(cirBuf, &el);
						sem_post(&(cirBuf->mutex));
						break;
								
							
					 case RD_DEVICE_STATUS:
						gettimeofday(&t2, NULL);
						/* obliczenie czasu dzialania programu */
						progTime = t2.tv_sec - t1.tv_sec;
							
						/* obliczenie czasu działania urządzenia */
						fproc = open("/proc/uptime", O_RDONLY);
						read(fproc , prBuf, sizeof(prBuf));
						close(fproc);
						ptime = strtok(prBuf, ".");
						devtime = atol(ptime);
						break;
						 
					 default: break;		
				 }
				 
				 /* składanie ramki odpowiedzi */
				 createModbusResponse(frame, resp, el, devtime, 
					                  progTime);
			 }
			 else {
				perror("Blad w ramce Modbus TCP/IP");
			 }
		 
			 retval = send(clisocket_fd, resp, MSGSIZE, 0); 
				 
			 if (retval<0) {
				 perror("Nie mozna wyslac komunikatu");
				 exit(1);
			 }
		 }while(retval != 0);
		 
		 close(clisocket_fd);
		 close(socket_fd);
		 exit(0);
	 }
	 /* błąd - nie udało się utworzyć procesu potomnego */
	 if (pid < 0){
		perror("nie udalo sie utworzyc procesu potomka");
		exit(-1);
	 }



	 signal(SIGINT, signalHandler);
	 
	 timecnt = 0;

	 /* proces macierzysty - obsługa pomiarów */
	 do {
		 
		 /* sprawdzenie czy wciśnięto przycisk SW1 */
		 gpioGetValue(&sw1, &status);
		 elem.inputDiscrete[0]  = status;
		 
		 /* sprawdzenie czy wciśnięto przycisk SW2 */
		 gpioGetValue(&sw2, &status);
		 elem.inputDiscrete[1]  = status;
		 
		 /* sprawdzenie stanu na wyjściu do które podłączona jest dioda
		  * czerwona LED1 */
		 gpioGetValue(&ledRed, &status);
		 elem.outputDiscrete[0] = status;
		 
		 /* pomiar temperatury i ciśnienia z czujnika ciśnienia BMP280*/
		 BMP280_Measure(&bmp280, &temp, &pres);
		 elem.holdRegisters[0] = temp;
		 elem.holdRegisters[1] = pres;
		 
		 /* odczytanie wartości analogowych z czujnika ADC w mV */
		 
		 /* odczyt wartości napięcia z potencjometru */
		 ain0 = adcReadValue(adcDriver, 0);
 
		 /* odczyt wartości napięcia z fotorezystora */
		 ain1 = adcReadValue(adcDriver, 1);
 
		 /* odczyt wartości napięcia z czujnika temperatury MCP9700A */
		 ain6 = adcReadValue(adcDriver, 6);

		 /* zapisanie wartości analogowych */
		 elem.inputRegisters[0] = ain0;
		 
		 lain  = ((double)(1800 - ain1))/ain1;
		 lain *=  (10.0/500.0);
		 lain  = pow(lain, -5.0/3.0);
		 
		 /* konwersja do lux */
		 elem.inputRegisters[1] = (int)lain;
		 
		 elem.inputRegisters[2] = (int)((((float)ain6 - 500.0)/10.0)
		                                                        *100.0);  
		 
		 /* odczyt wypełnienia */
		 pwmGetDutyCycle(&pwmRed, &duty);
		 elem.holdRegisters[2] = (int)(duty * 100.0);
		
		 /* pomiar przyspieszeń */
		 ADXL345_ReadData_XYZ(&adxl345, &accelX, &accelY, &accelZ);
		 elem.holdRegisters[3] = (int)(accelX * 100000.00);
		 elem.holdRegisters[4] = (int)(accelY * 100000.00);
		 elem.holdRegisters[5] = (int)(accelZ * 100000.00);
		 
		 /* pomiar temperatry i wilgotności z czujnika DHT11 */
		 if ( timecnt == 0 )
			read_dht(&dhtData, &dhtTemp, &dhtHum);	
			 
		 elem.holdRegisters[6] = (int)( dhtTemp * 100.0);
		 elem.holdRegisters[7] = (int)( dhtHum  * 100.0);
		 ++timecnt;
		 
		 if ( timecnt == 20 )
			timecnt = 0;
			
	 
		
		 sem_wait(&(cirBuf->mutex));
		 /* zablokowanie semafora na czas zapisu do pamięci */
		 
		 /* dodanie elementu pomiarowego do bufora cyklicznego */
		 addElem(cirBuf, elem);
		 
		 /* odblokowanie semafora */
		 sem_post(&(cirBuf->mutex));
		 
		 
		 
		 sleepms(configTime);
		 
	}while(interrflag==0);
	
	/* usunięcie utworzonego obszaru pamięci współdzielonej */
	shm_unlink("cirBuffer");
	
	/* czekanie na zakończnie utworzonego procesu */
	wait(&stat);
	
	/* usunięcie semafora */
	sem_close(&(cirBuf->mutex));
	 
	/* zamknięcie deskryptora do czujnika cisnienia */ 
	BMP280_Close(&bmp280);
	
	/* zamknięcie deskryptora do akcelerometru */
	ADXL345_Close(&adxl345); 
	 
	 
	return 0;
}
