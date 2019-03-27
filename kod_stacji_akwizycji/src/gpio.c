/*!
 * \file gpio.c
 * \author Rafał Januszewski
 *  
 * Utworzono: 21 paź 2017
 *
 */

#include "gpio.h"



int gpioEnable(gpio *gp, unsigned int num, int pollingTime)
{
	int  fd;              /* deskryptor pliku */
	char str[4];          /* łańcuch znaków określający numer pinu */

	/* otwórz plik export w trybie tylko do zapisu */
	fd = open("/sys/class/gpio/export", O_WRONLY );

	if (fd < 0 ) 
	{
		perror("gpio_export");
		return -1;
	}
	
	sprintf(str, "%d", num);
	
	/* zapisz wartość do pliku */
	write(fd, str, sizeof(str));
	gp->number  = num;
	gp->timeout = pollingTime;
	
	close(fd);      /* zamknij deskryptor */
	usleep(500000); /* opóźnienie dla reakcji systemu */

	return 0;
}




int gpioDisable(gpio *gp)
{
	int  fd;
	char str[4];         /* łańcuch znaków określający numer pinu */

	/* otwórz plik unexport w trybie tylko do zapisu */
	fd = open("/sys/class/gpio/unexport", O_WRONLY );

	if (fd < 0 )
	{
		perror("gpio_unexport");
		return -1;
	}
	
	sprintf(str, "%d", gp->number);
	
	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);

	return 0;
}




int gpioSetDirection(gpio *gp, char *dir)
{
	int  fd;
	char path[36];  /* zawierająca ścieżkę do pinu typu gpio */

	sprintf(path, "/sys/class/gpio/gpio%d/direction", gp->number);
	fd = open(path, O_WRONLY );

	if (fd < 0 )
	{
		perror("gpio_set_direction");
		return -1;
	}
	
	/* zapisz nazwę określającą kierunek do pliku direction */
	write(fd, dir, sizeof(dir));
	close(fd);

	usleep(500000);

	return 0;
}


int gpioGetDirection(gpio *gp, char *dir)
{
	int  fd;
	char path[36];

	sprintf(path, "/sys/class/gpio/gpio%d/direction", gp->number);
	fd = open(path, O_RDONLY );

	if (fd < 0 )
	{
		perror("gpio_get_direction");
		return -1;
	}
	
	read(fd, dir, 4);
	close(fd);
	usleep(500000);

	return 0;
}


int gpioSetValue(gpio *gp, unsigned int value)
{
	int  fd;
	char path[32];
	char val[2];

	sprintf(path, "/sys/class/gpio/gpio%d/value", gp->number);
	fd = open(path, O_WRONLY );

	if (fd < 0 )
	{
		perror("gpio_set_value");
		return -1;
	}
	
	sprintf(val, "%d", value);
	
	/* zapisz wartość 1 lub 0 do pliku value */
	write(fd, val, sizeof(val));
	close(fd);

	usleep(500000);

	return 0;
}



int gpioGetValue(gpio *gp, unsigned int *value)
{
	int  fd;
	char path[32];
	char val;

	sprintf(path, "/sys/class/gpio/gpio%d/value", gp->number);
	fd = open(path, O_RDONLY );

	if (fd < 0 )
	{
		perror("gpio_get_value");
		return -1;
	}
	read(fd, &val, sizeof(val));
	
	if (val == '0')
		*value = 0;
	else
		*value = 1;
		
	close(fd);  /* zamknij deskryptor */

	usleep(500000);

	return 0;
}



int gpioSetEdge(gpio *gp, char *edge)
{
	int fd;
	char path[35];

	sprintf(path, "/sys/class/gpio/gpio%d/edge", gp->number);
	fd = open(path, O_WRONLY );

	if (fd < 0 )
	{
		perror("gpio_set_edge");
		return -1;
	}

	write(fd, edge, sizeof(edge));

	close(fd);

	usleep(500000);

	return 0;
}



int gpioGetEdge(gpio *gp, char *edge)
{
	int  fd;
	char path[35];

	sprintf(path, "/sys/class/gpio/gpio%d/edge", gp->number);
	fd = open(path, O_RDONLY );

	if (fd < 0 )
	{
		perror("gpio_get_edge");
		return -1;
	}
	read(fd, edge, 7);

	close(fd);
	usleep(500000);

	return 0;
}




int gpioActiveLowChange(gpio *gp, unsigned int value)
{
	int fd;
	char path[35];
	char val[2];

	sprintf(path, "/sys/class/gpio/gpio%d/active_low", gp->number);
	fd = open(path, O_WRONLY );

	if (fd < 0 )
	{
		perror("gpio_activelow_change");
		return -1;
	}
	
	sprintf(val, "%d", value);
	write(fd, val, sizeof(val));
	close(fd);
	usleep( 500000 );

	return 0;
}


	
	
int gpioActiveLowRead(gpio *gp, unsigned int *value)
{
	int fd;
	char path[35];
	char val;

	sprintf(path, "/sys/class/gpio/gpio%d/active_low", gp->number);
	fd = open(path, O_RDONLY );

	if (fd < 0 )
	{
		perror("gpio_activelow_change");
		return -1;
	}
	
	
	read(fd, &val, sizeof(val));
	
	if (val == '0')
		*value = 0;
	else
		*value = 1;
		
	close(fd);
	usleep( 500000 );

	return 0;
}




int gpioPoll(gpio *gp)
{
	struct pollfd fdin;  /* struktura zawierająca deskryptor i liczbę */
	                     /* zarejetrowanych zadarzeń */
	int    retval;       /* wartość zwracana */
	int    fd;           /* deskryptor pliku gpio%/value */
	char   buffer[35];   /* bufor na ścieżkę do pliku */
	
	int    nfds = 1;      /* liczba obsługiwanych deskryptorów */
	
	int    pollTime = (gp->timeout) * 1000;  /* czas odpytywania [ms] */
	
	/* sprawdzenie czy czas odpytywania jest zerowy */
	if ((gp->timeout) < 1) {
		perror("timeout < 0");
		return -1;
	}
	
	sprintf(buffer, "/sys/class/gpio/gpio%d/value", gp->number);
	fd = open(buffer, O_RDONLY);
	
	if (fd < 0) {
		perror("opening file failed");
		return -1;
	}
	
	fdin.fd     = fd; 
	fdin.events = POLLPRI | POLLERR;
	
	retval = poll(&fdin, nfds, pollTime);
	
	if (retval < 0) {
		perror("gpio_poll");
		close(fd);
		return -1;
	}
	else if (retval == 0) {
		printf("Nie odnotowano zadnych przerwan\n");
		printf("w czasie %d ms\n", pollTime);
	}
	else {
		if (fdin.revents & (POLLPRI | POLLERR))
			read(fdin.fd, gp->events, MAX_EVENTS_BUF);
	}
	close(fd);
	return 0; 
}

