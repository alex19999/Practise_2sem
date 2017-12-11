all: subserver

split.o: split.c
	gcc -c -o split.o split.c
subserver: split.o subserver.o
	gcc -o subserver split.o  subserver.o -lpthread
