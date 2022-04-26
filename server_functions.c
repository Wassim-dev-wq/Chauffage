
#ifndef SERVER_FUNCTIONS_C
#define SERVER_FUNCTIONS_C
#include <sys/ipc.h>
#include <sys/msg.h>
#include "server.h"
#include "MessageTemperature.h"

#define MAX 10


// structure for message queue
struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;

/* TUTORIAL_BEGIN */
// Function that prints an error message to stderr and exits.
void error(const char *message)
{
  perror(message);
  exit(1);
}
/* TUTORIAL_END */

extern int tcpSockFD;
extern int udpSockFD;

/*This is to make sure the port does not remain in use after process is terminated
 *
 * Please note that SIGSTOP and SIGKILL cannot be caught or handled. */


void interruptHandler(int signalNo)
{
  close(tcpSockFD);
  close(udpSockFD);
  exit(0);
}

// Process a TCP connection and sends a confirmation message to the socket
void processTCPRequest(int socketFD)
{
  if (socketFD < 0)
  {
    return;
  }

  const size_t BUFFER_SIZE = 256;
  /* calloc() sets the memory to zero, so no need to call bzero here. */
  char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
  Message *msg = (struct Message*)malloc(sizeof(Message));
  printf("Server >> sizeof(msg) %ld",sizeof(msg));
  int n = (int)read(socketFD, msg, sizeof(msg));
  if (n < 0)
  {
    error("Error reading from socket");
  }

  /* TUTORIAL_BEGIN */
  printf("Here is the message from TCP socket: Numero piece %d\nType fonction %d\n", msg->num_piece,msg->type_fonction);
  // SEND DATA TO TO COMMUNICATION TEMPERATURE PUIS L'ENVOYER A CHAUFFAGE
//  printf("Message temperature envoyer\n");
  /* TUTORIAL_END */
//  writeIntoChauffage(buffer);

  // C Program for Message Queue (Writer Process)
	key_t key;
	int msgid;

	// ftok to generate unique key
	key = ftok("progfile", 65);

	// msgget creates a message queue
	// and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;

	printf("Write Data : ");
//	fgets(message.mesg_text,MAX,stdin);
  strcpy(message.mesg_text,"Cuisine Chauffer");
	// msgsnd to send message
	msgsnd(msgid, &message, sizeof(message), 0);

	// display the message
	printf("Data send is : %s \n", message.mesg_text);

//  const char *confirmationMessage = "I got your message";
          //  const char *confirmationMessage = "Message Temperature envoyer";
          // n = (int)write(socketFD, confirmationMessage, strlen(confirmationMessage));
  
  n = (int)write(socketFD, buffer, strlen(buffer));          

  if (n < 0)
  {
    error("Error writing to socket");
  }

  free(msg); /* Free buffer to prevent a memory leak. */
}

// Proces a UDP connection and sends a confirmation message
void processUDPRequest(int socketFD, struct sockaddr *address)
{
  if (socketFD < 0)
  {
    return;
  }

  const size_t BUFFER_SIZE = 256;
  /* calloc() sets the memory to zero, so no need to call bzero here. */
  char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));

  socklen_t addressLength = sizeof(*address);
  int numBytes = (int)recvfrom(socketFD, buffer, BUFFER_SIZE - 1, 0, address, &addressLength);

  if (numBytes < 0)
  {
    error("Error in recvfrom");
  }
  else if (numBytes == 0)
  {
    printf("No data received from UDP socket");
  }
  else
  {
    printf("Here is the message from UDP socket: %s\n", buffer);
    MessageTemperature *m = new_MessageTemperature(17,1,"chambre");
    printf("Message temperature envoyer\n");
  }

  const char *confirmationMessage = "I got your message";
  strcpy(buffer, confirmationMessage);

  numBytes = (int)sendto(socketFD, buffer, strlen(buffer), 0, address, addressLength);
  if (numBytes < 0)
  {
    error("Error in sendto");
  }

  free(buffer); /* Free buffer to prevent a memory leak. */
}

#endif
