all: main

matrix.o: matrix.c
	gcc -c -o matrix.o matrix.c -lpthread

main.o: main.c
	gcc -c -o main.o main.c -lpthread

main: matrix.o main.o 
	gcc -o main matrix.o main.o -lpthread
