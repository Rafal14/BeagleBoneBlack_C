/*!
 * \file adc.c
 * \author Rafał Januszewski
 *  
 * Utworzono: 21 paź 2017
 *
 */


#include "adc.h"


int enableAdc()
{
	int fd;

	char str[15];
	strcpy(str, "cape-bone-iio");

	fd = open("/sys/devices/bone_capemgr.9/slots", O_WRONLY);

	if ( fd < 0 ) {
		perror("open");
		return -1;
	}

	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);
	
	return 0;
}



int adcCheckDriverNo()
{
	FILE *fptr;       /* uchwyt do pliku */

	int driverNo;
	
	char *pdriver;
	
	char dir[60];
	char path[50];
	char str[3];

	sprintf(dir, "find /sys/devices/ocp.*/ -type d -name helper.*");
	
	fptr = popen(dir, "r");
	
	if ( fptr == NULL ) {
		perror("adcCheckDriverNo");
		return -1;
	}
	
	
	while (fgets(path, 50, fptr) != NULL);

	pdriver = strrchr(path, '.'); 
 

	if ( pdriver == NULL ) {
		perror("fgets");
		return -1;
	}	
	pclose(fptr);     /* zamknięcie strumienia */

	
	++pdriver;
	str[0] = (*pdriver);
	++pdriver;
	str[1] = (*pdriver);
	str[2] = '\n';
	
	driverNo = atoi(str);

	return driverNo;
}

	


int adcReadValue(int driver, unsigned int number)
{
	int fd;
	char readBuf[8];
	char path[30];
	
	int value;
	
	sprintf(path, "/sys/devices/ocp.3/helper.%d/AIN%d", driver, number);

	fd = open(path, O_RDONLY);

	if ( fd < 0 ) {
		perror("open");
		return -1;
	}

	read( fd , readBuf, 7 );
	
	value = atoi(readBuf);
	close(fd);
	usleep(500000);

	return value;
}
