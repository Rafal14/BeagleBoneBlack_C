/*!
 * \file   ADXL345.c
 * \author Rafał Januszewski
 *  
 * Utworzono: 28 paź 2017
 *
 */


#include "adxl345.h"





int ADXL345_Open(spidevice *sensor)
{
	if ( spiOpen(sensor, 1,0) < 0 ) {
		perror("ADXL345_Open");
		return -1;
	}
	return 0;
}



void ADXL345_Close(spidevice *sensor)
{
	spiClose(sensor);
}



void ADXL345_Config(spidevice *sensor)
{
	spiConfig(sensor, 3, 8, 500000);
	usleep(500000);
}




int ADXL345_Read_Id(spidevice *sensor)
{
	unsigned char rbuf[2];
	
	if ( spiReadData(sensor, ID_REGISTER, rbuf, 2) < 0 ) {
		perror("ADXL345_Read_Id");
		return -1;
	}

	return rbuf[1];
}



int ADXL345_Enable_Measurement(spidevice *sensor) 
{
	unsigned char sbuf[2], rbuf[2];
	
	memset(sbuf, 0, sizeof(sbuf));
	
	sbuf[0]    = POWER_CTL;
	sbuf[1]    = 0x08;
	if (spiWriteData(sensor, sbuf, rbuf, 2) < 0) {
		perror("ADXL345_Enable_Measurement");
		return -1;
	}
	usleep(500000);
	return 0;
}


int ADXL345_SetFullResolution(spidevice *sensor) 
{
	unsigned char sbuf[2], rbuf[2];
	
	memset(sbuf, 0, sizeof(sbuf));
	
	sbuf[0]    = DATA_FORMAT;
	sbuf[1]    = 0x08;
	if (spiWriteData(sensor, sbuf, rbuf, 2) < 0) {
		perror("ADXL345_Enable_Measurement");
		return -1;
	}
	usleep(500000);
	return 0;
}




int ADXL345_ReadData_XYZ(spidevice *sensor, double *x, double *y, 
                                                       double *z)
{
	unsigned char buf[7];
	
	short rawX, rawY, rawZ;
	
	if (spiReadData(sensor, DATA_START, buf, 7) < 0) {
		perror("ADXL345_ReadData_XYZ");
		return -1;
	}
	
	
	rawX = ((short)buf[2]>>8) | (short)buf[1];

	rawY = ((short)buf[4]>>8) | (short)buf[3];
	
	rawZ = ((short)buf[6]>>8) | (short)buf[5];

	
	/* konwersja do m/s^2 */
	*x = (double)rawX * 0.0039 * 9.81;  /* 3.9 mG/LSB */
	*y = (double)rawY * 0.0039 * 9.81;
	*z = (double)rawZ * 0.0039 * 9.81;
	
	
	return 0;
}

