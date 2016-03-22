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
    int sockfd,waitfd,n;
    socklen_t sock_length;
    struct sockaddr_in INET_Server_address;
    struct sockaddr_in INET_Client_address;
    char ip_address[100], bufptr[100];

    if ( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror ("socket");
      return -1;
    }

    bzero (&INET_Server_address, sizeof (INET_Server_address));
    bzero (&INET_Client_address, sizeof (INET_Client_address));

    INET_Server_address.sin_family = AF_INET;
    INET_Server_address.sin_port = htons (atoi (argv[2]));
    inet_pton (AF_INET, argv[1], &(INET_Server_address.sin_addr));

    if (bind (sockfd, (struct sockaddr *) &INET_Server_address, sizeof (struct sockaddr_in)) < 0)
    {
      perror ("bind");
      return -1;
    }
    printf ("bind() system call is successful.\n");

    sock_length = sizeof (INET_Server_address);
    getsockname (sockfd, (struct sockaddr *) &INET_Server_address, &sock_length);
    inet_ntop (AF_INET, &(INET_Server_address.sin_addr), ip_address, 100);

    printf ("socket IP = %s, socket Port = %hu\n", ip_address,  ntohs (INET_Server_address.sin_port));


    listen (sockfd, 5);
    sock_length = sizeof (INET_Client_address);

    for ( ; ; )
    {
        waitfd = accept (sockfd, (struct sockaddr *) &INET_Client_address, &sock_length);
        if (waitfd < 0)
          {
           perror ("accept");
           return -1;
          }

        inet_ntop (AF_INET, &(INET_Client_address.sin_addr), ip_address, 100);
        printf ("accepted a request from the client with IP = %s and Port = %hu\n", 
                ip_address, ntohs (INET_Server_address.sin_port));

        bzero (bufptr, 100*sizeof (char));
        n = recv (waitfd, &bufptr, 100, 0);
        printf ("server: message from client = %s\n", bufptr);
        n = send (waitfd, &bufptr, 100, 0);

        shutdown (waitfd, SHUT_RDWR);
    }

    shutdown (sockfd, SHUT_RDWR);
    return 0;

}
