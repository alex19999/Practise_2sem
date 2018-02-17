all: main

main: words_counter.o main.o 
	g++ words_counter.o main.o -o main

words_counter.o: words_counter.cpp
	g++ -c words_counter.cpp

main.o: main.cpp
	g++ -c main.cpp
