

#include "chauffage.h"
#define HELLO_PORT 1025
#define HELLO_GROUP "233.1.1.1"
#define MSG_BUFF_SIZE 50
// CHAUFFAGE

typedef struct User
{
    char nom[30];
    int age;
} User;

// void UDPConnection(MessageTemperature *m, char *arg)
// {
//     u_int yes = 1;
//     int send_s, recv_s; /* Sockets for sending and receiving. */
//     u_char ttl;
//     struct sockaddr_in mcast_group;
//     struct ip_mreq mreq;
//     struct sockaddr_in from;
//     memset(&mcast_group, 0, sizeof(mcast_group));
//     mcast_group.sin_family = AF_INET;
//     mcast_group.sin_port = htons(1024);
//     mcast_group.sin_addr.s_addr = inet_addr(arg);
//     if ((recv_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
//     {
//         perror("recv socket");
//         exit(1);
//     }

//     if (setsockopt(recv_s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
//     {
//         perror("reuseaddr setsockopt");
//         exit(1);
//     }

//     if (bind(recv_s, (struct sockaddr *)&mcast_group, sizeof(mcast_group)) < 0)
//     {
//         perror("bind");
//         exit(1);
//     }

//     /* Preparatios for using Multicast */
//     mreq.imr_multiaddr = mcast_group.sin_addr;

//     /* Tell the kernel we want to join that multicast group. */
//     if (setsockopt(recv_s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
//     {
//         perror("add_membership setsockopt");
//         exit(1);
//     }
//     while (1)
//     {
//         byte *data = toBytes(m);
//         int nbytes = sendto(recv_s, data, sizeof(data), 0, (struct sockaddr *)&from, sizeof(from));
//         printf("op\n");
//         sleep(1);
//         if (nbytes < 0)
//         {
//             perror("sendto");
//             return 1;
//         }
//     }
// }

void UDPConnection(char *buffer, int bufferSize)
{

    struct sockaddr_in addr;
    int fd, cnt;
    struct ip_mreq mreq;
    char *message=buffer;
    MessageTemperature *m = new_MessageTemperature(17,1,"Cuisine");
    byte* data = toBytes(m);
    /* create what looks like an ordinary UDP socket */
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        perror("socket");
        exit(1);
    }

    /* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(HELLO_GROUP);
    addr.sin_port=htons(HELLO_PORT);

    if (sendto(fd,data,sizeof(data),0,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
      perror("sendto");
      exit(1);
    }
    // if (numBytes < 0)
    // {
    //     error("Client could not write to UDP socket");
    // }
    // numBytes = recvfrom(socketFD, buffer, bufferSize, 0, serverAddress, &serverAddressLength);
    // if (numBytes < 0)
    // {
    //     error("Client could not read from UDP socket");
    // }
    /* Print server's response */
    // printf("%s\n", buffer);
    //free(buffer); /* Free buffer to prevent a memory leak. */
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;

    //MessageTemperature *m = new_MessageTemperature(12, 0, "Cuisine");
    char *nomPiece;
    // printf("%s",getPiece(m));
    if (argc < 2)
    {
        fprintf(stderr, "Erreur, le nom du piéce n'est pas mentionner \n");
        exit(1);
    }
    nomPiece = argv[1];
    printf("%s\n", argv[1]);
    const size_t BUFFER_SIZE = 256;
    /* calloc() sets the memory to zero, so no need to call bzero here.a*/
    char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrClient;
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(3000);
    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    printf("connecté\n");

    User user;
    // while (1)
    // {
    char msg[MSG_BUFF_SIZE];
    recv(socketClient, msg, MSG_BUFF_SIZE, 0);
    // printf("Piece a envoyer %s\nSize %ld",m->ops->getPiece(m),sizeof(m));
    printf("Server >> %s\n", msg);
    UDPConnection(msg, MSG_BUFF_SIZE);
    // scanf("%s %d",user.nom,&user.age);
    //send(socketClient, &m, sizeof(m), 0);
    // }

    close(socketClient);
    return 0;
}