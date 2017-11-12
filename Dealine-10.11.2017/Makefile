all: main

split.o: split.c
	gcc -c -o split.o split.c

wash_wipe.o: wash_wipe.c
	gcc -c -o wash_wipe.o wash_wipe.c

main: split.o wash_wipe.o main.o 
	gcc -o main split.o wash_wipe.o main.o -lpthread
