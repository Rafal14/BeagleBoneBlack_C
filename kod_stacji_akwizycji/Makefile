# Plik makefile - wpisz 'make gpio' w celu skompilowania calego programu
# prosciej mozna wpisac 'make'

CC=gcc
CFLAGS=-g -Iinc -Wall -pedantic

__bbb__:	bbb
			./bbb

bbb:		obj obj/main.o obj/gpio.o obj/adc.o obj/pwm.o \
            obj/i2cdevice.o obj/bmp280.o obj/buffer.o obj/modtcp.o \
            obj/spidevice.o obj/adxl345.o obj/mmgpio.o obj/timedht.o \
            obj/dht11.o
			$(CC) $(CFLAGS) obj/main.o obj/gpio.o obj/adc.o obj/pwm.o \
			obj/i2cdevice.o obj/bmp280.o obj/buffer.o obj/modtcp.o \
			obj/spidevice.o obj/adxl345.o obj/mmgpio.o obj/timedht.o \
			obj/dht11.o -o bbb -lrt -lpthread -lm
	    
obj:
			mkdir obj
	    
obj/main.o:	src/main.c inc/gpio.h inc/adc.h inc/pwm.h inc/buffer.h \
			inc/modtcp.h inc/spidevice.h inc/adxl345.h inc/mmgpio.h \
			inc/dht11.h inc/timedht.h
			$(CC) -c $(CFLAGS) -o obj/main.o src/main.c 

obj/gpio.o:	src/gpio.c inc/gpio.h
			$(CC) -c $(CFLAGS) -o obj/gpio.o src/gpio.c
	    
obj/adc.o:	src/adc.c inc/adc.h
			$(CC) -c $(CFLAGS) -o obj/adc.o src/adc.c
	    
obj/pwm.o:	src/pwm.c inc/pwm.h
			$(CC) -c $(CFLAGS) -o obj/pwm.o src/pwm.c
		
obj/i2cdevice.o:src/i2cdevice.c inc/i2cdevice.h
			$(CC) -c $(CFLAGS) -o obj/i2cdevice.o src/i2cdevice.c
		
obj/bmp280.o:src/bmp280.c inc/bmp280.h inc/i2cdevice.h
			$(CC) -c $(CFLAGS) -o obj/bmp280.o src/bmp280.c
				
obj/buffer.o:src/buffer.c inc/buffer.h
			$(CC) -c $(CFLAGS) -o obj/buffer.o src/buffer.c
		
obj/modtcp.o:src/modtcp.c inc/modtcp.h inc/buffer.h
			$(CC) -c $(CFLAGS) -o obj/modtcp.o src/modtcp.c
		
obj/spidevice.o:src/spidevice.c inc/spidevice.h
			$(CC) -c $(CFLAGS) -o obj/spidevice.o src/spidevice.c

obj/adxl345.o:src/adxl345.c inc/adxl345.h inc/spidevice.h
			$(CC) -c $(CFLAGS) -o obj/adxl345.o src/adxl345.c
			
obj/mmgpio.o:src/mmgpio.c inc/mmgpio.h
			$(CC) -c $(CFLAGS) -o obj/mmgpio.o src/mmgpio.c
			
obj/timedht.o:src/timedht.c inc/timedht.h
			$(CC) -c $(CFLAGS) -o obj/timedht.o src/timedht.c
			
obj/dht11.o:src/dht11.c inc/dht11.h inc/mmgpio.h inc/timedht.h
			$(CC) -c $(CFLAGS) -o obj/dht11.o src/dht11.c

clean:
		rm -f obj/*.o bbb
		rm -rf obj
