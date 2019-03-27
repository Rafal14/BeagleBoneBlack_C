/*!
 * \file   spidevice.c
 * \author Rafał Januszewski
 *  
 * Utworzono: 28 paź 2017
 *
 */


#include "spidevice.h"



int enableSPI(int busNumber)
{
	int fd;

	char str[12];
	
	sprintf(str, "BB-SPIDEV%d", busNumber);

	fd = open("/sys/devices/bone_capemgr.9/slots", O_WRONLY);

	if ( fd < 0 ) {
		perror(str);
		return -1;
	}

	write(fd, str, sizeof(str));
	close(fd);
	usleep(500000);

	return 0;
}



int spiOpen(spidevice *dev, int busNumber, int csNumber)
{
	int  fd;
	char path[14];
	
	sprintf(path, "/dev/spidev%d.%d", busNumber, csNumber);

	fd = open(path, O_RDWR);

	if ( fd < 0 ) {
		perror(path);
		return -1;
	}
	
	/* zapisuje deskryptor (uchwyt) do magistrali 
	 * spi* w strukurze urządzenia */
	dev->fd    = fd;
	dev->busNo = busNumber;
	dev->csNo  = csNumber;

	return fd;
}



void spiClose(spidevice *dev)
{
	/* kończy połączenie z magistralą spi* */
	close(dev->fd);
}



int spiConfig(spidevice *dev, uint8_t mode, uint8_t bitNum,
              uint32_t speed)
{
	int fd = dev->fd;
	
	dev->usec_delay    =  1;
    dev->speed         =  speed;    
    dev->bits_per_word =  bitNum;
	
	/* określenie trybu odczytu i zapisu danych */
	if ( ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0 ) {
		perror("spiConfig: SPI_IOC_WR_MODE");
		return -1;
	}
	
	if ( ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0 ) {
		perror("spiConfig: SPI_IOC_RD_MODE");
		return -1;
	}
	
	/* określenie liczby bitów na słowo w ramce danych */
	if ( ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitNum) < 0 ) {
		perror("spiConfig: SPI_IOC_WR_MODE");
		return -1;
	}
	
	if ( ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bitNum) < 0 ) {
		perror("spiConfig: SPI_IOC_RD_MODE");
		return -1;
	}
	
	/* określenie prędkości przesyłania danych */
	if ( ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0 ) {
		perror("spiConfig: SPI_IOC_WR_MODE");
		return -1;
	}
	
	if ( ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0 ) {
		perror("spiConfig: SPI_IOC_RD_MODE");
		return -1;
	}
	
	return 0;
}



int spiReadData(spidevice *dev,  unsigned char  addrReg,
                                 unsigned char *readBuf, int size)
{
	int fd = dev->fd;
	
	unsigned char sendBuf[size];
	
	struct spi_ioc_transfer data;
	
	
	memset(sendBuf, 0, sizeof(sendBuf));
	
	addrReg |= 0x80;
	
	if (size > 2) {
		addrReg |= 0x40;    /* odczytaj wiele bajtów */
	}
	
	sendBuf[0] = addrReg;
	
	data.tx_buf        = (unsigned long) sendBuf;
	data.rx_buf        = (unsigned long) readBuf;
	data.len           =  size;
	data.delay_usecs   =  dev->usec_delay;
    data.speed_hz      =  dev->speed;
    data.bits_per_word =  dev->bits_per_word;
	
	if (ioctl(fd, SPI_IOC_MESSAGE(1), &data) == -1) {
		return -1;
	}	
	
	return 0;
}


        
                
int spiWriteData(spidevice *dev, unsigned char *sendBuf, 
                                 unsigned char *readBuf, int size)
{
	int fd = dev->fd;
	
	struct spi_ioc_transfer data;
	
	if (size > 2) {
		sendBuf[0] |= 0x40;    /* odczytaj wiele bajtów */
	}
	
	data.tx_buf        = (unsigned long) sendBuf;
	data.rx_buf        = (unsigned long) readBuf;
	data.len           =  size;
	data.delay_usecs   =  dev->usec_delay;
    data.speed_hz      =  dev->speed;
    data.bits_per_word =  dev->bits_per_word;
	
	if (ioctl(fd, SPI_IOC_MESSAGE(1), &data) == -1) {
		return -1;
	}	
	
	return 0;
}
