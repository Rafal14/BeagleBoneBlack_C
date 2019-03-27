/*!
 * \file   i2cdevice.c
 * \author Rafał Januszewski
 *  
 * Utworzono: 21 paź 2017
 *
 */


#include "i2cdevice.h"



int enableI2C(int busNumber)
{
	int fd;

	char str[8];
	
	sprintf(str, "BB-I2C%d", busNumber);

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



int i2cOpen(i2cdevice *dev, int busNumber)
{
	int  fd;
	char path[10];
	
	sprintf(path, "/dev/i2c-%d", busNumber);

	fd = open(path, O_RDWR);

	if ( fd < 0 ) {
		perror(path);
		return -1;
	}
	
	/* zapisuje deskryptor (uchwyt) do magistrali 
	 * I2C* w strukurze urządzenia */
	dev->fd    = fd;
	dev->busNo = busNumber;

	return fd;
}



void i2cClose(i2cdevice *dev)
{
	/* kończy połączenie z magistralą I2C* */
	close(dev->fd);
}




int i2cConfig(i2cdevice *dev, int8_t addr)
{
	int  fdesc = dev->fd;
	
	/* przypisanie adresu slave'owi */
	if ( ioctl(fdesc, I2C_SLAVE, addr) < 0 ) {
		perror("sensor");
		return -1;
	}
	
	dev->addrDev = addr;
	
	return 0;
}




int i2cReadData(i2cdevice *dev, unsigned char regAddr, char *readBuf,
                int bufSize)
{
	int fd = dev->fd;
	
	if ( write(fd, &regAddr, 1) != 1 ) {
		perror("i2cSendData");
		return -1;
	}
	
	if ( read(fd, readBuf, bufSize) != bufSize ) {
		perror("i2cReadData");
		return -1;
	}
	
	return 0;
}




int i2cWriteData(i2cdevice *dev, char *writeBuf, int writeBufSize)
{
	int fd = dev->fd;

	if ( write(fd, writeBuf, writeBufSize) != writeBufSize ) {
		perror("i2cWriteData");
		return -1;
	}
	
	return 0;
}
