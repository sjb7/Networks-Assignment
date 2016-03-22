#include	<sys/types.h>		
#include	<sys/socket.h>		
#include	<stdlib.h>		
#include	<string.h>		
#include	<stdio.h>		
#include	<sys/un.h>		
#include	<unistd.h>	
#include	<arpa/inet.h>	

int main(int argc, char *argv[])
{
    int sockfd, n;
    socklen_t addrlen = sizeof (struct sockaddr_in);
    struct sockaddr_in INET_Server_address, INET_Client_address;
    char bufptr[100], ip_address[100];

    if ( (sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror ("socket");
      return -1;
    }

    bzero (&INET_Server_address, addrlen);
    bzero (&INET_Client_address, addrlen);

 
    INET_Server_address.sin_family = AF_INET;
    INET_Server_address.sin_port = htons (atoi (argv[2]));
    inet_pton (AF_INET, argv[1], &(INET_Server_address.sin_addr));

    if (bind (sockfd, (struct sockaddr *) &INET_Server_address, addrlen) < 0)
    {
      perror ("bind");
      return -1;
    }

    n = recvfrom (sockfd, &bufptr, 100, 0, (struct sockaddr *) &INET_Client_address, &addrlen);
    inet_ntop (AF_INET, &(INET_Client_address.sin_addr), ip_address, 100);
    printf ("server: received message from the client with IP = %s and Port = %hu\n", 
            ip_address, ntohs (INET_Client_address.sin_port));
    printf ("server: message from client = %s\n", bufptr);
    n = sendto (sockfd, &bufptr, n, 0, (struct sockaddr *) &INET_Client_address, addrlen);

    shutdown (sockfd, SHUT_RDWR);
    return 0;

}
