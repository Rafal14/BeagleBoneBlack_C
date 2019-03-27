/*!
 * \file   bmp280.c
 * \author Rafał Januszewski
 *  
 * Utworzono: 28 paź 2017
 *
 */


#include "bmp280.h"





int BMP280_Open(i2cdevice *sensor)
{
	if ( i2cOpen(sensor, 2) < 0 ) {
		perror("BMP280_Open");
		return -1;
	}
	return 0;
}



void BMP280_Close(i2cdevice *sensor)
{
	i2cClose(sensor);
}



void BMP280_Config(i2cdevice *sensor)
{
	i2cConfig(sensor, BMP280_ADDR);
}




int BMP280_Read_Id(i2cdevice *sensor, char *buf)
{
	if ( i2cReadData(sensor, ID_REG, buf, 1) < 0 ) {
		perror("BMP280_Read_Id");
		return -1;
	}
	
	return 0;
}



int BMP280_ReadCtrMeas(i2cdevice *sensor, char *buf)
{
	if ( i2cReadData(sensor, CTRL_REG, buf, 1) < 0 ) {
		perror("BMP280_ReadCtrMeas");
		return -1;
	}
	
	return 0;
}




int BMP280_WriteCtrMeas(i2cdevice *sensor)
{
	char buf[2];
	
	buf[0] = CTRL_REG;    /* adres rejestru konfiguracji pomiarów  */

	/* Zawartość rejestru ctrl_meas
	 *  ------------------------------
	 * |  osrs_t |  osrs_p  | mode_t  | temp - 16 bit / 0.0050 °C
	 * |  0 0 1  | 0   1  0 |  1 1    | ciśnienie  - 17 bit / 1.31 Pa
	 *  ------------------------------  mode_te = normal mode
	 */
	buf[1] = 0x2B;   /* T - x1,   p - x2 */        
	
	if ( i2cWriteData(sensor, buf, 2) < 0 ) {
		perror("BMP280_WriteCtrMeas");
		return -1;
	}
	
	return 0;
}





int BMP280_ReadConfigReg(i2cdevice *sensor, char *buf)
{
	if ( i2cReadData(sensor, CONFIG_REG, buf, 1) < 0 ) {
		perror("BMP280_ReadConfigReg");
		return -1;
	}
	
	return 0;
}





int BMP280_WriteConfigReg(i2cdevice *sensor)
{
	char buf[2];
	
	buf[0] = CONFIG_REG;    /* adres rejestru configuracji pomiarów */


	/* Zawartość rejestru config
	 *  -----------------------------------------------------
	 * |time standby|  filter  | reserved | spi3wire_enable | 
	 * | 1   0    0 | 0   0  0 |    0     |       0         |
	 *  ----------------------------------------------------
	 * time standby = 500 ms
	 * filter - wspołczynnik filtra IIR brak
	 */
	buf[1] = 0x80;   

	
	if ( i2cWriteData(sensor, buf, 2) < 0 ) {
		perror("BMP280_WriteConfigReg");
		return -1;
	}
	
	return 0;
}




int BMP280_ReadCalib(i2cdevice *sensor, char *buf, int bufsize)
{
	if ( i2cReadData(sensor, CALIB_REG, buf, bufsize) < 0 ) {
		perror("BMP280_Read_Id");
		return -1;
	}
	
	return 0;
}



int BMP280_ReadRawData(i2cdevice *sensor, char *buf, int bufsize)
{
	if ( i2cReadData(sensor, RAW_DATA_REG, buf, bufsize) < 0 ) {
		perror("BMP280_Read_Id");
		return -1;
	}
	
	return 0;
}




int BMP280_Measure(i2cdevice *sensor, long *temperature,
                   unsigned long *pressure)
{
	char calBuf[24];
	char rawDataBuf[6];
	
	int dig_T1, dig_T2, dig_T3;
	int dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8;
	int dig_P9;
	
	long adc_T, adc_p;
	
	long var1, var2;
	
	long t_fine;
	
	unsigned long pres;
	
	/* odczytaj współczynniki temperatury i ciśnienia */
	BMP280_ReadCalib(sensor, calBuf, 24);
	
	dig_T1 = calBuf[1] * 256 + calBuf[0];
	dig_T2 = calBuf[3] * 256 + calBuf[2];
	if(dig_T2 > 32767)
	{
		dig_T2 -= 65536;
	}
	
	dig_T3 = calBuf[5] * 256 + calBuf[4];
	if(dig_T3 > 32767)
	{
		dig_T3 -= 65536;
	}

	dig_P1 = calBuf[7] * 256 + calBuf[6];
	dig_P2 = calBuf[9] * 256 + calBuf[8];
	if(dig_P2 > 32767)
	{
		dig_P2 -= 65536;
	}
	
	dig_P3 = calBuf[11]* 256 + calBuf[10];
	if(dig_P3 > 32767)
	{
		dig_P3 -= 65536;
	}
	
	dig_P4 = calBuf[13]* 256 + calBuf[12];
	if(dig_P4 > 32767)
	{
		dig_P4 -= 65536;
	}
	
	dig_P5 = calBuf[15]* 256 + calBuf[14];
	if(dig_P5 > 32767)
	{
		dig_P5 -= 65536;
	}
	
	dig_P6 = calBuf[17]* 256 + calBuf[16];
	if(dig_P6 > 32767)
	{
		dig_P6 -= 65536;
	}
	
	dig_P7 = calBuf[19]* 256 + calBuf[18];
	if(dig_P7 > 32767)
	{
		dig_P7 -= 65536;
	}
	
	dig_P8 = calBuf[21]* 256 + calBuf[20];
	if(dig_P8 > 32767)
	{
		dig_P8 -= 65536;
	}
	
	dig_P9 = calBuf[23]* 256 + calBuf[22];
	if(dig_P9 > 32767)
	{
		dig_P9 -= 65536;
	}
	
	
	/* odczytaj wartości temp i ciśnienia - nieprzetworzone */
	BMP280_ReadRawData(sensor, rawDataBuf, 6);
	
	
	/* 32-bitowy long signed */
	adc_p = ((long)rawDataBuf[0]) * 4096 + ((long)rawDataBuf[1]) * 16 
	        + ((long)(rawDataBuf[1] & 0xF0))/16;  
	
	
	adc_T = ((long)rawDataBuf[3]) * 4096 + ((long)rawDataBuf[4]) * 16 
	         + ((long)(rawDataBuf[5] & 0xF0))/16;
	         
	         
	
    /* obliczenia stałoliczbowe */
    var1 = (((adc_T >> 3) - ((long)dig_T1 << 1)) * (long)dig_T2) >> 11;
    var2 = (((((adc_T >> 4) - (long)dig_T1) * ((adc_T >> 4) 
                - (long)dig_T1)) >> 12) * (long)dig_T3 ) >> 14;

	t_fine = var1 + var2;
	
	*temperature = (t_fine * 5 + 128) >> 8;
	
	
	var1 = (((long)t_fine)>>1) - (long)64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11 ) * ((long)dig_P6);
	var2 = var2 + ((var1 * ((long)dig_P5)) << 1);
	var2 = (var2 >> 2)+ (((long)dig_P4) << 16);
	var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13 )) >> 3) 
	          + ((((long)dig_P2) * var1) >> 1)) >> 18;
	var1 =((((32768 + var1))*((long)dig_P1)) >> 15);

	if (var1 == 0)
	{
		return 0; /* uniknięcie dzielenia przez zero */
	}

	pres = (((unsigned long)(((long)1048576)-adc_p)-(var2 >> 12)))*3125;
	if (pres < 0x80000000)
	{
		pres = (pres << 1) / ((unsigned long)var1);
	}
	else
	{
		pres = (pres / (unsigned long)var1) * 2;
	}
	var1 = (((long)dig_P9) * ((long)(((pres >> 3) * (pres >> 3)) 
	          >> 13))) >> 12;
	var2 = (((long)(pres >> 2)) * ((long)dig_P8)) >> 13;
	pres = (unsigned long)((long)pres + ((var1 + var2 + dig_P7) >> 4));
	
	
	*pressure = pres;
    
	
	return 0;
}

