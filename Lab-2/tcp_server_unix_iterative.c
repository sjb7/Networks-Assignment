#include	<sys/types.h>		
#include	<sys/socket.h>		
#include	<stdlib.h>		
#include	<string.h>		
#include	<stdio.h>		
#include	<sys/un.h>		
#include	<unistd.h>		

int main(int argc, char *argv[])
{
    int sockfd, workfd, n;
    socklen_t sock_length;
    struct sockaddr_un UNIX_Server_address, UNIX_Client_address;
    char bufptr[100];

    if ( (sockfd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
      perror ("socket");
      return -1;
    }

    bzero (&UNIX_Server_address, sizeof (UNIX_Server_address));
    bzero (&UNIX_Client_address, sizeof (UNIX_Client_address));

 
    UNIX_Server_address.sun_family = AF_LOCAL;
    unlink (argv[1]);
    strncpy (UNIX_Server_address.sun_path, argv[1], sizeof (UNIX_Server_address.sun_path)-1);

    if (bind (sockfd, (struct sockaddr *) &UNIX_Server_address, sizeof (struct sockaddr_un)) < 0)
    {
      perror ("bind");
      return -1;
    }

    bzero (&UNIX_Server_address, sizeof (UNIX_Server_address));
    sock_length = sizeof (UNIX_Server_address);
    getsockname (sockfd, (struct sockaddr *) &UNIX_Server_address, &sock_length);

 
    listen (sockfd, 5);

    for ( ; ; )
    {
 
        workfd = accept (sockfd, (struct sockaddr *) &UNIX_Client_address, &sock_length);
        if (workfd < 0)
          {
           perror ("accept");
           return -1;
          }
   
        n = recv (workfd, &bufptr, 100, 0);
        printf ("server: message from client = %s\n", bufptr);
        n = send (workfd, &bufptr, 100, 0);

        shutdown (workfd, SHUT_RDWR);
    }

    shutdown (sockfd, SHUT_RDWR);
    return 0;

}
