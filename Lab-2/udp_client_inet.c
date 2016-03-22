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
    char bufptr[100];

    if ( (sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror ("socket");
      return -1;
    }

    bzero (&INET_Server_address, addrlen);
    bzero (&INET_Client_address, addrlen);

   
    bzero (bufptr, 100*sizeof (char));
    printf ("client: message to server = ");
    scanf ("%[^\n]", bufptr);

   
    INET_Server_address.sin_family = AF_INET;
    INET_Server_address.sin_port = htons (atoi (argv[2]));
    inet_pton (AF_INET, argv[1], &(INET_Server_address.sin_addr));

    n = sendto (sockfd, &bufptr, strlen (bufptr) + 1, 0, (struct sockaddr *) &INET_Server_address, addrlen);
    printf ("server: message from client = %s\n", bufptr);
    bzero (bufptr, 100*sizeof (char));
    n = recvfrom (sockfd, &bufptr, n, 0, (struct sockaddr *) &INET_Server_address, &addrlen);
    printf ("client: echo from server = %s\n", bufptr);

    shutdown (sockfd, SHUT_RDWR);
    return 0;

}
