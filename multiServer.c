#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include "MessageTemperature.h"
#include "server.h"
// COMMUNICATION TEMPERATURE
struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;
typedef struct User
{
    char nom[30];
    int age;
} User;

void *function(void *arg)
{
    int socket = *(int *)arg;
    const char *msg = "Cuisine";
    User user;
   // send(socket, msg, strlen(msg) + 1, 0);
	key_t key;
	int msgid;

	// ftok to generate unique key
	key = ftok("progfile", 65);

	// msgget creates a message queue
	// and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);

	// msgrcv to receive message
	msgrcv(msgid, &message, sizeof(message), 1, 0);

	// display the message
	printf("Data Received is : %s \n",message.mesg_text);
    send(socket,message.mesg_text,strlen(message.mesg_text)+1,0);
	// to destroy the message queue
	msgctl(msgid, IPC_RMID, NULL);
    //int nbbytes = recv(socket, &m, sizeof(m), 0);
    //printf("Le client s'appelle %s et a %d ans\n", user.nom, user.age);
    close(socket);
    free(arg);
    pthread_exit(NULL);
}

int main(void)
{
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(3000);

    bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    printf("bind : %d\n", socketServer);

    listen(socketServer, 5);
    printf("listen\n");

    pthread_t threads[5];
    for (int i = 0; i < 5; i++)
    {
        struct sockaddr_in addrClient;
        socklen_t csize = sizeof(addrClient);
        int socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
        printf("accept\n");
        printf("client : %d\n", socketClient);

        int *arg = malloc(sizeof(int));
        *arg = socketClient;
        pthread_create(&threads[i], NULL, function, arg);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }

    close(socketServer);
    printf("close\n");
    return 0;
}