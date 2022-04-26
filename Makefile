#Two step process of compiling and making executable files for server and client
#Author: Fortythree Shiaohongtu

all: server client


server: server.o server_functions.o server.h MessageTemperature.c 
	gcc -o server server.o server_functions.o server.h MessageTemperature.c 
server.o: server.c 
	gcc -c server.c
server_functions.o: server_functions.c MessageTemperature.c multiServer.o
	gcc -c server_functions.c MessageTemperature.c multiServer.c
multiServer.o: multiServer.c
	gcc -c multiServer.c
client: client.o client_functions.o client.h
	gcc client.c client_functions.c -o client
client.o: client.c
	gcc -c client.c
client_functions.o: client_functions.c
	gcc -c client_functions.c
clean:
	rm -rf server client server.o server_functions.o client.o client_functions.o
