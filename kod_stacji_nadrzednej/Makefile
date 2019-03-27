# 
# Plik dla kompilacji oprogramowania stacji nadrzędnej
#
# Plik makefile - wpisz 'make cli' w celu skompilowania calego programu
# prosciej mozna wpisac 'make'

CC=gcc
CFLAGS=-g -Iinc -Wall -pedantic

cli:		obj obj/main.o obj/modbus.o
			$(CC) $(CFLAGS) obj/main.o obj/modbus.o -o cli -lrt -lpthread
			
obj:
			mkdir obj
			
obj/main.o:	src/main.c inc/modbus.h
			$(CC) -c $(CFLAGS) -o obj/main.o src/main.c
	    
obj/modbus.o:src/modbus.c inc/modbus.h
			$(CC) -c $(CFLAGS) -o obj/modbus.o src/modbus.c
	    
clean:
		rm -f obj/*.o cli
		rm -rf obj
