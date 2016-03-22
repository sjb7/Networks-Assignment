#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#define MY_PORT		9999
#define MAXBUF		1024

int main(int Count, char *Strings[])
{   int sockfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

	
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}

	
	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
	{
		perror("socket--bind");
		exit(errno);
	}

	
	if ( listen(sockfd, 20) != 0 )
	{
		perror("socket--listen");
		exit(errno);
	}

	
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);

	
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	while (1){
                 bzero(&buffer, sizeof(buffer));
		 int size = recv(clientfd, buffer, sizeof(buffer),0);
                 int i = 0;
                 int j = strlen(buffer) - 1;
                 int flag = 0;
                 while(i<j){
                     if(buffer[i++]!=buffer[j--]){
                         flag = 1;
                         break;
                     }
                 }
             
                 char str[15];
                 if(flag == 0){strcpy(str,"yes");}
                 else{strcpy(str,"no");}
		 send(clientfd, str, sizeof(str), 0);

	
	}

	
	close(sockfd);
	return 0;
}

