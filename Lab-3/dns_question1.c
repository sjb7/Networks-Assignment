#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "macros.h"

#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *res;
    int error;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;   
    hints.ai_socktype = 0; 		
    hints.ai_flags = AI_PASSIVE;   
    hints.ai_protocol = 0;        
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

   
    error = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (error != 0)
    {   
        fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
        return EXIT_FAILURE;
    }   

    char ipv4addr[INET_ADDRSTRLEN] = "";
    char ipv6addr[INET6_ADDRSTRLEN] = "";
    char hostname[NI_MAXHOST] = "";
    
    for (res = result; res != NULL; res = res->ai_next)
    {   
	if (res->ai_family == AF_INET) {
		inet_ntop(AF_INET, &(((struct sockaddr_in *)res->ai_addr)->sin_addr), ipv4addr, INET_ADDRSTRLEN);
		printf("%s,%s,%s,%s\n", ai_family_string(res->ai_family), ai_socktype_string(res->ai_socktype), 
			ai_protocol_string(res->ai_protocol), ipv4addr);
	}
	if (res->ai_family == AF_INET6) {
		inet_ntop(AF_INET6, &(((struct sockaddr_in *)res->ai_addr)->sin_addr), ipv6addr, INET6_ADDRSTRLEN);
		printf("%s,%s,%s,%s\n", ai_family_string(res->ai_family), ai_socktype_string(res->ai_socktype), 
			ai_protocol_string(res->ai_protocol), ipv6addr);
	}

        error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0); 
        if (error != 0)
        {
            fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
            continue;
        }
        if (*hostname != '\0')
            printf("hostname: %s\n", hostname);
    }   

    freeaddrinfo(result);
    return EXIT_SUCCESS;
}


