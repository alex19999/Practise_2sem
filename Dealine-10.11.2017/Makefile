all: wash_and_dry

split.o: split.c
	gcc -c -o split.o split.c

wash_and_dry.o: wash_and_dry.c
	gcc -c -o wash_and_dry.o wash_and_dry.c

wash_and_dry: split.o wash_and_dry.o 
	gcc -o wash_and_dry split.o wash_and_dry.o -lpthread
