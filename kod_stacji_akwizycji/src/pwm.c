/*
 * pwm.c
 *
 *  Created on: 19 sie 2017
 *      Author: rafal
 */

#include "pwm.h"


int enablePwmControl()
{
	int fd;

	char str[15];
	strcpy(str, "am33xx_pwm");

	fd = open("/sys/devices/bone_capemgr.9/slots", O_WRONLY);

	if ( fd < 0 ) {
		perror("enablePwmControl");
		return -1;
	}

	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);

	return 0;
}


int enablePwmPin(pwm *pw, unsigned int portNo, unsigned int pinNo)
{
	int fd;

	char str[20];

	pw->socketNo = portNo;
	pw->number   = pinNo;


	if ( portNo == 9 ) {
		if (pinNo == 21) {
			return 0;
		}
	}

	sprintf(str, "bone_pwm_P%d_%d", portNo, pinNo);

	fd = open("/sys/devices/bone_capemgr.9/slots", O_WRONLY);

	if ( fd < 0 ) {
		perror("enablePwmPin");
		return -1;
	}

	write(fd, str, sizeof(str));
	close(fd);

	usleep(500000);

	return 0;
}


int pwmCheckDriverNo(pwm *pw)
{
	FILE *fptr;       /* uchwyt do pliku */

	int driverNo;
	
	char *pdriver;
	
	char dir[60];
	char path[50];
	char str[3];

	sprintf(dir, "find /sys/devices/ocp.*/ -name pwm_test_P%d_%d.*", 
                pw->socketNo, pw->number);
	
	fptr = popen(dir, "r");
	
	if ( fptr == NULL ) {
		perror("pwmcheckPinDriver");
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
	
	pw->driver   = driverNo;
	return 0;
}



int pwmSetPolarity(pwm *pw, unsigned int polarity)
{
	int fd;

	char path[50];
	char str[2];

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/polarity",
	        pw->socketNo, pw->number, pw->driver);
	
	sprintf(str, "%d", polarity);

	fd = open(path, O_WRONLY);

	if ( fd < 0 ) {
		perror("pwmSetPolarity");
		return -1;
	}

	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);
	return 0;
}



int pwmGetPolarity(pwm *pw, unsigned int *polarity)
{
	int fd;

	char path[50];
	char pol;

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/polarity",
	        pw->socketNo, pw->number, pw->driver);
	
	fd = open(path, O_RDONLY);

	if ( fd < 0 ) {
		perror("pwmGetPolarity");
		return -1;
	}

	read(fd, &pol, sizeof(pol));
	
	if (pol == '0') 
		*polarity = 0;
	else 
		*polarity = 1;
	
	close(fd);
	usleep(500000);
	return 0;
}





int pwmSetPeriod(pwm *pw, long timePeriod)
{
	int fd;

	char path[50];
	char str[12];

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/period",
	        pw->socketNo, pw->number, pw->driver);
	sprintf(str, "%ld", timePeriod);

	fd = open(path, O_WRONLY);

	if ( fd < 0 ) {
		perror("pwmSetPeriod");
		return -1;
	}

	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);
	
	pw->period = timePeriod;

	return 0;
}




int pwmGetPeriod(pwm *pw, long *timePeriod)
{
	int fd;

	char path[50];
	char buf[12];

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/period", 
	        pw->socketNo, pw->number, pw->driver);

	fd = open(path, O_RDONLY);

	if ( fd < 0 ) {
		perror("pwmGetPeriod");
		return -1;
	}
	read(fd, buf, 11);

	*timePeriod = atoi(buf);

	close(fd);

	usleep( 500000 );

	return 0;
}



int pwmSetTimeDuty(pwm *pw, long timeDuty)
{
	int fd;

	char path[50];
	char str[12];

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/duty", 
	        pw->socketNo, pw->number, pw->driver);
	sprintf(str, "%ld", timeDuty);

	fd = open(path, O_WRONLY);

	if ( fd < 0 ) {
		perror("pwmSetTimeDuty");
		return -1;
	}

	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);	

	return 0;
}


int pwmGetTimeDuty(pwm *pw, long *timeDuty)
{
	int fd;

	char path[50];
	char buf[12];

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/duty", 
	        pw->socketNo, pw->number, pw->driver);
	        
	fd = open(path, O_RDONLY);

	if ( fd < 0 ) {
		perror("pwmGetTimeDuty");
		return -1;
	}

	read(fd, buf, 11);

	*timeDuty = atoi(buf);

	close(fd);

	usleep(500000);

	return 0;
}




int pwmSetDutyCycle(pwm *pw, float dutyCycle)
{
	int fd;

	char path[50];
	char str[20];
	
	long timeDut, period;
	
	period = pw->period;
	
	timeDut = (long)(((float)period) * dutyCycle);

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/duty", 
	        pw->socketNo, pw->number, pw->driver);
	        
	sprintf(str, "%ld", timeDut);

	fd = open(path, O_WRONLY);

	if ( fd < 0 ) {
		perror("pwmSetDutyCycle");
		return -1;
	}

	write(fd, str, sizeof(str));


	close(fd);

	usleep( 500000 );

	return 0;
}






int pwmGetDutyCycle(pwm *pw, float *dutyCycle)
{
	int  fd;
	long timeDuty;    /* czas wypełnienia w ms */
	long totalPeriod = pw->period;

	char path[50];  /* bufor na ścieżkę do pliku */
	char buf[12];   /* bufor do odczytu czasu wypełnienia */

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/duty", 
	        pw->socketNo, pw->number, pw->driver);

	fd = open(path, O_RDONLY);

	if ( fd < 0 ) {
		perror("GetDutyCycle");
		return -1;
	}

	read(fd, buf, 11);

	timeDuty   = atoi(buf);
	*dutyCycle = ((float)timeDuty) / ((float)totalPeriod);  

	close(fd);

	usleep(500000);

	return 0;
}



int pwmRun(pwm *pw, unsigned int mode)
{
	int fd;
	char path[50];  
	char str[2];

	sprintf(path, "/sys/devices/ocp.3/pwm_test_P%d_%d.%d/run", 
	        pw->socketNo, pw->number, pw->driver);
	        
	fd = open(path, O_WRONLY);
	
	if ( fd < 0 ) {
		perror("run");
		return -1;
	}
	
	sprintf(str, "%d", mode);
	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);
	
	return 0;
}
