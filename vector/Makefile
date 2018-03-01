all: main

main: vector3.o main.o 
	g++ vector3.o main.o -o main

vector3.o: vector3.cpp
	g++ -c vector3.cpp

main.o: main.cpp
	g++ -c main.cpp
