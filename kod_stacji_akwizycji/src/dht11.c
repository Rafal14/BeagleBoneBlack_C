
#include <string.h>
#include <stdlib.h>

#include "dht11.h"



#define NO_PULSES  41

#define MAX_PULSES 32000



int read_dht(mmgpio *pinData, float* temp, float* humidity) {
	
	
	uint32_t counter = 0;
	
	uint32_t threshold = 0;
	
	int pulses[NO_PULSES*2] = {0};
	
	uint8_t data[5] = {0};
	
	uint8_t sum;
	
	int i, ind;

	volatile int j;
	
	
	*temp = 0.0f;
	*humidity = 0.0f;
	
	
	mm_set_output(pinData);
	
	schedSetPriority();
	
	mm_set_bit(pinData);
	
	sleepms(500);
	
	mm_clear_bit(pinData);
	delayTransmition(20);
	
	
	mm_set_input(pinData);

	for (j=0; j < 70; ++j) {
	}

	while ( mm_get_value(pinData) ) {
		if ( ++counter >= MAX_PULSES ) {
			backNormalPriority();
			return -1;
		}
	}
	
	
	for (i=0; i < NO_PULSES*2; i+=2) {
		
		while (!mm_get_value(pinData)) {
			if ( ++pulses[i] >= MAX_PULSES ) {
				backNormalPriority();
				return -2;
			}
		}
		
		while (mm_get_value(pinData)) {
			if ( ++pulses[i+1] >= MAX_PULSES ) {
				backNormalPriority();
				return -3;
			}
		}
	}

	backNormalPriority();
	
	
	for (i=2; i < NO_PULSES*2; i+=2) {
		threshold += pulses[i];
	}
	threshold /= NO_PULSES-1;
	
	
	
	for (i=3; i < NO_PULSES*2; i+=2) {
		ind = (i-3)/16;
		data[ind] <<= 1;
		if (pulses[i] >= threshold) {
			data[ind] |= 1;
		}
	}
	
	sum = data[0] + data[1] + data[2] + data[3];
	if (data[4] != (sum & 0xff)) {
		return -5;
	}
	
	*temp = (float)data[2];
	*humidity = (float)data[0];
	
	return 0;
}
