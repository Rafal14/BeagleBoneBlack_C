#include <stdint.h>
#include <stdio.h>

#include "modtcp.h"


int checkModbusFrame(uint8_t *query, uint8_t *fnc, uint16_t *addr,
                                                   uint16_t *val)
{
	/* sprawdzenie poprawności ramki*/
	if (query[2] != 0x00) {
		if (query[3] != 0x00) {
			
			query[7] += 0x80;
			return -1;
		}
	}
	else { 
		if (query[6] != 0xff) {
			
			query[7] += 0x80;
			return -1;
		}
	}
	
	/* kod funkcji */
	*fnc = query[7];
	
	
	/* skopiuj wartość adresu */ 
	*addr = query[8] * 256 + query[9];

	/* skopiuj wartość */ 
	*val  =	query[10] * 256 + query[11];
	
	
	return 0;
}



void createModbusResponse(uint8_t *query, uint8_t *response, 
                         value_t elem, uint32_t uptime, uint32_t prtime)
{
	/* kod funkcji */
	uint8_t fCode = query[7];
	
	uint16_t len, amount, addr;
	
	/* liczba zliczonych bajtów */
	uint8_t countBytes=0;
	int     i,j;
	int     k=0;
	
	
	/* kopiowanie numeru transakcji */
	response[0] = query[0];
	response[1] = query[1];
	
	/* kopiowanie numeru określającego typ protokołu */
	response[2] = query[2];
	response[3] = query[3];
	
	/* kopiowanie indentyfikatora jednostki */
	response[6] = query[6];
	
	addr = query[8] * 256 + query[9];
	
	
	/* obliczenie długości ramki odpowiedzi */
	if ( (fCode == READ_COILS) || (fCode == READ_DISCRETE_INPUTS) ) {
		
		/* liczba wyjść do odczytu */
		amount = query[10] * 256 + query[11];
		
		if (amount < 8) {
			countBytes = 1;
		} 
		else {
			if ( (amount % 8) > 0 ) {
				countBytes = amount / 8;
				++countBytes;
			}
			else {
				countBytes = amount;
			}
		}
		
		len = countBytes + 3;
		
		response[4] = len >> 8; 
		response[5] = len & 0x00ff;
		
		
		/* liczba zliczonych bajtów */
		response[8] = countBytes;
		
		for(i=0,j=9; i<countBytes; i++, j++) {
			response[j] = 0;
			for (k=0; k < amount; k++) {
				if ( fCode == READ_COILS ) {
					if ( elem.outputDiscrete[k] == 1 )
					  response[j] |= (1 << k);
					else
					  response[j] &= ~(1 << k);
				}
				if ( fCode == READ_DISCRETE_INPUTS ) {
					if ( elem.inputDiscrete[k] == 1 )
					  response[j] |= (1 << k);
					else
					  response[j] &= ~(1 << k);
				} 
			}
		}
	}
	
	
	if ((fCode == READ_INPUT_REGISTER) || (fCode == READ_HOLDING_REGISTERS)) {
		
		
		/* liczba rejestrów wejściowych do odczytu */
		amount = query[10] * 256 + query[11];
		
		countBytes = amount * 2;
		
		if (fCode == READ_HOLDING_REGISTERS)
			amount = amount / 2;
		
		
		/* zapisz długość */	
		len = countBytes + 3;
		response[4] = len >> 8; 
		response[5] = len & 0x00ff;
		
		/* liczba zliczonych bajtów */
		response[8] = countBytes;
		
		if ( addr == 48 ) { 
			k = 2;
		}
		
		if ( addr == 60 ) {
			k = 3;
		}
		
		if ( addr == 80 ) {
			k = 6;
		}
		
		for(i=k,j=9; i<(amount+k); i++, j+=2) {
			if ( fCode == READ_INPUT_REGISTER ) {
				response[j]   = (elem.inputRegisters[i]) >> 8;
				response[j+1] = (elem.inputRegisters[i]) & 0x00ff;
			}
			
			if ( fCode == READ_HOLDING_REGISTERS ) {
				response[j]   = (elem.holdRegisters[i]) >> 24;
				response[j+1] = (elem.holdRegisters[i]) >> 16;
				response[j+2] = (elem.holdRegisters[i] & 0xff00) >> 8;
				response[j+3] = (elem.holdRegisters[i]) & 0x00ff;
				j+=2;
			}
		}
	}
	
	
	
	if ( fCode == WRITE_SINGLE_COIL ) {
		
		
		/* skopiuj wartość adresu */ 
		response[8] = query[8];
		response[9] = query[9];
		
		/* skopiuj wartość adresu */ 
		response[10] = query[10];
		response[11] = query[11];
		
		
		len = 6;
		
		response[4] = len >> 8; 
		response[5] = len & 0x00ff;
	}
	
	
	
	/* odczytaj status urządzenia - czas działania programu i czas
	 * działania urządzenia */
	if ( fCode == RD_DEVICE_STATUS ) {
		
		len = 11;
		response[4] = len >> 8; 
		response[5] = len & 0x00ff;
		
		/* liczba zliczonych bajtów */
		response[8]  = 8;
		
		/* zapis czasu pracy urządzenia na 32 bitach */
		response[9]  =  uptime >> 24;
		response[10] =  uptime >> 16;
		response[11] = (uptime & 0xff00) >> 8;
		response[12] =  uptime & 0x00ff;
		
		/* zapis czasu działania programu na 32 bitach */
		response[13] =  prtime >> 24;
		response[14] =  prtime >> 16;
		response[15] = (prtime & 0xff00) >> 8;
		response[16] =  prtime & 0x00ff;
		
	}
	
	if ( fCode == NONSTOP_MEASURE ) {
		
		countBytes = 0;    /* licznik bajtów */
		
		/* zapis stanu wyjścia binarnego */
		for(i=0,j=9; i<1; i++, j++) {
			response[j] = 0;
			for (k=0; k < 1; k++) { 
				if (elem.outputDiscrete[k] == 1) {
				  response[j] |= (1 << k);
				}
				else {
				  response[j] &= ~(1 << k);
				}
			}
		}
		++countBytes;
		
		/* zapis stanu wejść binarnych */
		for(i=0; i<1; i++, j++) {
			response[j] = 0;
			for (k=0; k < 2; k++) {
				if (elem.inputDiscrete[k] ==1) {
				  response[j] |= (1 << k);
				}
				else {
				  response[j] &= ~(1 << k);
				}
			}
		}
		++countBytes;
		
		
		/* zapis wartości z wejść analogowych */
		for(i=0; i<3; i++, j+=2) {
			response[j]   = (elem.inputRegisters[i]) >> 8;
			response[j+1] = (elem.inputRegisters[i]) & 0x00ff;
			
			countBytes += 2;
		}
		
		
		/* zapis wartości z rejestrów */
		for(i=0; i<8; i++, j+=4) {
			response[j]   = (elem.holdRegisters[i]) >> 24;
			response[j+1] = (elem.holdRegisters[i]) >> 16;
			response[j+2] = (elem.holdRegisters[i] & 0xff00) >> 8;
			response[j+3] = (elem.holdRegisters[i]) & 0x00ff;
			
			countBytes += 4;
		}
		
		
		response[8]  = countBytes;
		
		
		len = countBytes + 3;
		response[4] = len >> 8; 
		response[5] = len & 0x00ff;
	}
	
	
	
	response[7] = fCode;   /* kod funkcji */
}
