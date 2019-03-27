#include <stdint.h>
#include <stdio.h>

#include "modbus.h"


/* funkcja do składania ramki Modbusa TCP IP */
void createModbusRequest(uint8_t *buf, uint16_t *tranId, 
                         uint8_t funCode, uint16_t startAddr, 
                         uint16_t numReg, uint16_t numRegWr) {
	
	uint16_t len = 6;          /* domyślna dlugość */
	
	
	/* MBAP Header - nagłówek ramki - bajty od 0 do 6 */
	
	/* Zapisz numer transakcji na dwóch bajtach */
	buf[0] = *tranId >> 8;      /*wyodrębnij najstarszy bajt */
	buf[1] = *tranId & 0x00ff;  /*wyodrębnij najmłodszy bajt */
	
	++(*tranId);  
	
	/* identyfikator protokołu (2 bajty) - 0x0000 - Modbus TCP/IP */
	buf[2] = 0x00; 
	buf[3] = 0x00; 
	
	/* długość części danych ramki + bajt jednost ident. (w bajtach) */
	if (funCode == RDWR_MULTIPLE_REGISTERS) {
		len = 8;
	}
	else if (funCode == WRITE_MULTIPLE_REGISTERS) {
		len = (numRegWr * 2) + 7;
		
		buf[8] = startAddr >> 8;      /*wyodrębnij najstarszy bajt */
		buf[9] = startAddr & 0x00ff;  /*wyodrębnij najmłodszy bajt */
		
		buf[10] = numRegWr >> 8;      /*wyodrębnij najstarszy bajt */
		buf[11] = numRegWr & 0x00ff;  /*wyodrębnij najmłodszy bajt */
		
		buf[12] = (numRegWr*2);

		if (numRegWr == 2) {
			buf[13] =  numReg           >> 24;
			buf[14] =  numReg           >> 16;
			buf[15] = (numReg & 0xff00) >> 8;
			buf[16] =  numReg & 0x00ff;
		}
		
	}
	else if (funCode == RDWR_MULTIPLE_REGISTERS) {
		len = (numRegWr * 2)+ 10;
	}
	else if ( (funCode == RD_DEVICE_STATUS) || 
	                                    (funCode == NONSTOP_MEASURE)) {
		len = 2;
	}
	else {
		buf[8] = startAddr >> 8;      /*wyodrębnij najstarszy bajt */
		buf[9] = startAddr & 0x00ff;  /*wyodrębnij najmłodszy bajt */
		
		buf[10] = numReg >> 8;      /*wyodrębnij najstarszy bajt */
		buf[11] = numReg & 0x00ff;  /*wyodrębnij najmłodszy bajt */
		
	}
	buf[4] = len >> 8;      /*wyodrębnij najstarszy bajt */
	buf[5] = len & 0x00ff;  /*wyodrębnij najmłodszy bajt */
	
	/* indentyfikator - wartość 0xff lub 0x00 - nieuzywany bajt */
	buf[6] = 0xff; 
	
	
	buf[7] = funCode;  /* kod funkcji */
	
}






int decodeModbusFrame(uint8_t *buf, uint16_t dataAddr) {
	
	uint8_t  fnc;
	uint32_t val;
	
	uint16_t addr, stat, ain;
	
	float  aTemp;
	
	double temperature, pressure, accX, accY, accZ;
	
	/* sprawdzenie poprawności ramki */
	if (buf[2] != 0x00) {
		if (buf[3] != 0x00) {
			
			buf[7] += 0x80;
			return -1;
		}
	}
	else { 
		if (buf[6] != 0xff) {
			return -1;
		}
	}
	
	/* kod funkcji */
	fnc = buf[7];

	if ( (fnc & 0xf0) == 0x80 ) {
		printf("wystapil wyjatek\n");
		return -1;
	}


	/* skopiuj wartość */ 
	val  =	buf[9];
	


	switch(fnc) {
		default: break;
		
		case READ_COILS: 
			if (val == 1) {
				printf("Dioda czerwona LED1 jest zapalona\n\n");
			}
			if (val == 0) {
				printf("Dioda czerwona LED1 jest zgaszona\n\n");
			}
			break;
			
		case READ_DISCRETE_INPUTS:
		
			switch(val) {
				
				case 0:
					printf("Stan wejscia SW1: 0\n");
					printf("Stan wejscia SW2: 0\n\n");
					break;
				
				case 1:
					printf("Stan wejscia SW1: 1\n");
					printf("Stan wejscia SW2: 0\n\n");
					break;
					
				case 2:
					printf("Stan wejscia SW1: 0\n");
					printf("Stan wejscia SW2: 1\n\n");
					break;
					
				case 3:
					printf("Stan wejscia SW1: 1\n");
					printf("Stan wejscia SW2: 1\n\n");
					break;
					
				default: break;
			}
			break;
			
		case READ_INPUT_REGISTER:
			ain = buf[9] * 256 + buf[10];
			printf("Wartosc napiecia z potencjometru: %d mV\n", ain);
			
			ain = buf[11] * 256 + buf[12];
			printf("Wartosc natezenia swiatla z fotorezystora: %d lux\n", ain);
			
			ain = buf[13] * 256 + buf[14];

			aTemp = ((float)ain)/100.0;  
			printf("Wartosc temperatury z czujnika MCP9700: %.1f st.C\n\n",
			         aTemp);
			break;
			
		case READ_HOLDING_REGISTERS:
			
			if (dataAddr == 52) {
				if (buf[8] == 8) {
					val  =  buf[9]  << 24;
					val += (buf[10] << 16);
					val += (buf[11] << 8);
					val +=  buf[12];
					temperature = (double)val / 100.0;
					printf("Temperatura z czujnika BMP280:\t%.2f st.C\n", 
			                                          temperature);
			 
					val  =  buf[13] << 24;
					val += (buf[14] << 16);
					val += (buf[15] << 8);
					val +=  buf[16];
					pressure = (double)val / 100.0;
					printf("Cisnienie z czujnika BMP280:\t%.2f hPa\n\n",
			                                           pressure); 
				}
			}
			
			if (dataAddr == 48) {
				if (buf[8] == 4) {
					val  =  buf[9]  << 24;
					val += (buf[10] << 16);
					val += (buf[11] << 8);
					val +=  buf[12];
					printf("Wypelenienie sygnalu PWM:\t%d %%\n\n", 
															val);
				}
			}
			
			if (dataAddr == 60) {
				if (buf[8] == 12) {
					val  =  buf[9]  << 24;
					val += (buf[10] << 16);
					val += (buf[11] << 8);
					val +=  buf[12];
					accX = (double)val / 100000.00;
					printf("Przyspieszenie wzgledem osi X:\t%.2f m/(s^2)\n", 
			                                          accX);
			 
					val  =  buf[13] << 24;
					val += (buf[14] << 16);
					val += (buf[15] << 8);
					val +=  buf[16];
					accY = (double)val / 100000.00;
					printf("Przyspieszenie wzgledem osi Y:\t%.2f m/(s^2)\n",
			                                           accY); 
			                                           
					val  =  buf[17] << 24;
					val += (buf[18] << 16);
					val += (buf[19] << 8);
					val +=  buf[20];
					accZ = (double)val / 100000.00;
					printf("Przyspieszenie wzgledem osi Z:\t%.2f m/(s^2)\n\n",
			                                           accZ);                                        
			   }
			}
			
			if (dataAddr == 80) {
				if (buf[8] == 8) {
					val  =  buf[9]  << 24;
					val += (buf[10] << 16);
					val += (buf[11] << 8);
					val +=  buf[12];
					temperature = (double)val / 100.0;
					printf("Temperatura z czujnika DHT11:\t%.2f st.C\n", 
			                                          temperature);
			 
					val  =  buf[13] << 24;
					val += (buf[14] << 16);
					val += (buf[15] << 8);
					val +=  buf[16];
					pressure = (double)val / 100.0;
					printf("Wilgotność z czujnika DHT11:\t%.2f %%\n\n",
			                                           pressure); 
				}
			}
			
			break;
			
		case RD_DEVICE_STATUS:
			
			val  =  buf[9]  << 24;
			val += (buf[10] << 16);
			val += (buf[11] << 8);
			val +=  buf[12];
			printf("Czas dzialania urzadzenia:\t%d s\n", val);
			 
			val  =  buf[13] << 24;
			val += (buf[14] << 16);
			val += (buf[15] << 8);
			val +=  buf[16];
			printf("Czas dzialania programu:\t%d s\n\n", val); 
			break;
			
		case WRITE_SINGLE_COIL:
		
			addr = buf[8]  * 256 + buf[9];
			stat = buf[10] * 256 + buf[11];
		
			if (addr == 0) {
				if ( stat == 0xff00 ) 
					printf("Zapalono czerwona diode LED1\n\n");
				if ( stat == 0x0000 )
					printf("Zagaszono czerwona diode LED1\n\n");
			}	
			break;
			
		case WRITE_SINGLE_REGISTER:
			break;
			
			
		/* pomiar ciagly */
		case NONSTOP_MEASURE:
			
			if (val == 1) {
				printf("Dioda czerwona LED1 jest zapalona\n\n");
			}
			if (val == 0) {
				printf("Dioda czerwona LED1 jest zgaszona\n\n");
			}
			
			switch(buf[10]) {
				case 0:
					printf("Stan wejscia SW1: 0\n");
					printf("Stan wejscia SW2: 0\n\n");
					break;
				
				case 1:
					printf("Stan wejscia SW1: 1\n");
					printf("Stan wejscia SW2: 0\n\n");
					break;
					
				case 2:
					printf("Stan wejscia SW1: 0\n");
					printf("Stan wejscia SW2: 1\n\n");
					break;
					
				case 3:
					printf("Stan wejscia SW1: 1\n");
					printf("Stan wejscia SW2: 1\n\n");
					break;
				
				default: printf("Stan nieokreslony\n\n"); break;
			}
			
			ain = buf[11] * 256 + buf[12];
			printf("Wartosc napięcia z potencjometru: %d mV\n", ain);
			
			ain = buf[13] * 256 + buf[14];
			printf("Wartosc natezenia swiatla z fotorezystora: %d lux\n", 
			                                                       ain);
			
			ain = buf[15] * 256 + buf[16];
			
			aTemp = ((float)ain)/100.0;  
			printf("Wartosc temperatury z czujnika MCP9700: %.1f st.C\n\n",
			         aTemp);
			

			val  =  buf[17]  << 24;
			val += (buf[18] << 16);
			val += (buf[19] << 8);
			val +=  buf[20];
			temperature = (double)val / 100.0;
			printf("Temperatura z czujnika BMP280:\t%.2f st.C\n", 
			                                          temperature);
			 
			val  =  buf[21] << 24;
			val += (buf[22] << 16);
			val += (buf[23] << 8);
			val +=  buf[24];
			pressure = (double)val / 100.0;
			printf("Cisnienie z czujnika BMP280:\t%.2f hPa\n\n",
			                                           pressure); 
			
			val  =  buf[25]  << 24;
			val += (buf[26] << 16);
			val += (buf[27] << 8);
			val +=  buf[28];
			printf("Wypelenienie sygnalu PWM:\t%d %%\n\n", 
														val);
		
			
			val  =  buf[29]  << 24;
			val += (buf[30] << 16);
			val += (buf[31] << 8);
			val +=  buf[32];
			accX = (double)val / 100000.00;
			printf("Przyspieszenie wzgledem osi X:\t%.2f m/(s^2)\n", 
		                                          accX);
			 
			val  =  buf[33] << 24;
			val += (buf[34] << 16);
			val += (buf[35] << 8);
			val +=  buf[36];
			accY = (double)val / 100000.00;
			printf("Przyspieszenie wzgledem osi Y:\t%.2f m/(s^2)\n",
			                                           accY); 
			                                           
			val  =  buf[37] << 24;
			val += (buf[38] << 16);
			val += (buf[39] << 8);
			val +=  buf[40];
			accZ = (double)val / 100000.00;
			printf("Przyspieszenie wzgledem osi Z:\t%.2f m/(s^2)\n\n",
			                                           accZ);          
			                                           
			val  =  buf[41]  << 24;
			val += (buf[42] << 16);
			val += (buf[43] << 8);
			val +=  buf[44];
			temperature = (double)val / 100.0;
			printf("Temperatura z czujnika DHT11:\t%.2f st.C\n", 
			                                          temperature);
			 
			val  =  buf[45] << 24;
			val += (buf[46] << 16);
			val += (buf[47] << 8);
			val +=  buf[48];
			pressure = (double)val / 100.0;
			printf("Wilgotność z czujnika DHT11:\t%.2f %%\n\n",
			                                           pressure); 
			break;	
	}
	
	return 0;
}

