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
    int sockfd,n;
    socklen_t addrlen;
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

    addrlen = sizeof (INET_Server_address);
    connect (sockfd, (struct sockaddr *) &INET_Server_address, addrlen);
    getpeername (sockfd, (struct sockaddr *) &INET_Server_address, &addrlen);
    inet_ntop (AF_INET, &(INET_Server_address.sin_addr), ip_address, 100);
    printf ("Connected to server with IP = %s and  Port = %hu\n", ip_address,  ntohs (INET_Server_address.sin_port));

    bzero (bufptr, 100*sizeof (char));
    printf ("client: message to server = ");
    scanf ("%[^\n]", bufptr);

    n = send (sockfd, bufptr, strlen (bufptr) + 1, 0);
    bzero (bufptr, 100*sizeof (char));
    recv (sockfd, bufptr, 100, 0);
    printf ("client: echo from server = %s\n", bufptr);

    shutdown (sockfd, SHUT_RDWR);
    return 0;

}
