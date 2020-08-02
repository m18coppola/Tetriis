# -*-Makefile-*-

CC=gcc
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g

all: main

main: main.o
	$(CC) main.o $(FLAGS) -o app.bin

main.o: main.c
	$(CC) main.c $(FLAGS) -c -o main.o

clean:
	rm -f *.o *.bin

