all: task_two

split.o: split.c
	gcc -c -o split.o split.c

task_two.o: task_two.c
	gcc -c -o task_two.o task_two.c

task_two: split.o task_two.o 
	gcc -o task_two split.o task_two.o
