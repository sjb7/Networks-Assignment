 #include	<sys/types.h>		
#include	<sys/socket.h>		
#include	<stdlib.h>		
#include	<stdio.h>		
#include	<string.h>		
#include	<sys/un.h>		

int main (int argc, char *argv[])
{
    int sockfd, n;
    char bufptr[100];
    struct sockaddr_un UNIX_Server_address, UNIX_Client_address;
    socklen_t addrlen;
    
    if ( (sockfd = socket ( AF_LOCAL, SOCK_STREAM, 0)) < 0)
    {
      perror ("socket");
      return -1;
    }

 
    addrlen = sizeof (struct sockaddr_un);
    bzero (&UNIX_Server_address, addrlen);
    bzero (&UNIX_Client_address, addrlen);

    UNIX_Server_address.sun_family = AF_LOCAL;
    strncpy (UNIX_Server_address.sun_path, argv[1], sizeof (UNIX_Server_address.sun_path)-1);
    if ( connect (sockfd, (struct sockaddr *) &UNIX_Server_address, addrlen) < 0)
      {
       perror ("connect");
       return -1;
      }

 
    printf ("client: message to server = ");
    scanf ("%[^\n]", bufptr);

 
    n = send (sockfd, bufptr, strlen (bufptr) + 1, 0);
    bzero (bufptr, 100*sizeof (char));
    recv (sockfd, bufptr, 100, 0);
    printf ("client: echo from server = %s\n", bufptr);

    shutdown (sockfd, SHUT_RDWR);
    return 0;
}
