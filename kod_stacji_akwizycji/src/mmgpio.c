/*!
 * \file   mmgpio.c
 *  
 * Utworzono: 13 lis 2017
 *
 */


#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>


#include "mmgpio.h"




int create_mm_gpio(mmgpio *gp, int controller, int number) {
	
	int       fd;
	volatile  uint32_t* gpio_contr = NULL;
	uint32_t  gpio_addr;
	
	/* sprawdź poprawność danych */
	if ( controller < 0 || controller > 3) {
		return -1;
	}
	if ( number < 0 || number > 31 ) {
		return -1;
	}
	
	/* utwórz dostęp do obszaru pamięci */
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if ( fd < 0 ) {
		return -2;
	}
	
	gp->fd = fd;
	
	switch(controller) {
		case 0:
			gpio_addr = GPIO0_ADDRESS;
			break;
			
		case 1:
			gpio_addr = GPIO1_ADDRESS;
			break;
			
		case 2:
			gpio_addr = GPIO2_ADDRESS;
			break;
			
		case 3:
			gpio_addr = GPIO3_ADDRESS;
			break;
			
		default: break;
	}
	
	
    /* Mapowanie obszaru pamięci GPIO */
    gpio_contr = (uint32_t*)mmap(NULL, GPIO_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, gpio_addr);
    
    if (gpio_contr == MAP_FAILED) {
      /* błąd mapowania pamięci */
      gpio_contr = NULL;
      return -3;
    }
  
	gp->contr  = gpio_contr;
	gp->number = number;

	return 0;
}


void mm_close(mmgpio *gp) {
	
	close(gp->fd);
}


void mm_set_bit(mmgpio *gp) {

	gp->contr[GPIO_SETDATAOUT/4] = 1 << (gp->number);
}


void mm_clear_bit(mmgpio *gp) {
	
	gp->contr[GPIO_CLEARDATAOUT/4] = 1 << (gp->number);
}




void mm_set_output(mmgpio *gp) {
	
	gp->contr[GPIO_OE/4] &= (0xFFFFFFFF ^ (1 << gp->number));	
}


void mm_set_input(mmgpio *gp) {
	
	gp->contr[GPIO_OE/4] |= (1 << gp->number);
}


uint32_t mm_get_value(mmgpio *gp) {
	
	return (gp->contr[GPIO_DATAIN/4]) & (1 << gp->number);
}

