all: server client

split.o: split.c
	gcc -c -o split.o split.c
client.o: client.c
	gcc -c -o client.o client.c
server: split.o server.o
	gcc -o server split.o  server.o
client: split.o client.o
	gcc -o client split.o client.o
clean:
	rm client server *.o
