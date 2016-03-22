
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>    

/*
struct hostent
{
        char *h_name;
        char **h_aliases;
        int h_addrtype;
        int h_length;
        char **h_addr_list;
};
*/
int main(int argc, char** argv)    
{
	struct sockaddr_in remote;    
	struct hostent *remoteHost;   
	char *host, *page;
	char buffer[BUFSIZ];
	int sock, connection;
	
	if(argc==1)  
	{	
		printf("Error - arguments\n");
		exit(1);
	}
	host = argv[1];
	
	if(argc > 2)
		page = argv[2];
	else
		page = "/";
	
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0) 
	{	
		printf("Error -- socket"); 
		exit(1);
	}
		
	remoteHost = gethostbyname(argv[1]);
	
	bcopy((char*)remoteHost->h_addr, (char*)&remote.sin_addr, remoteHost->h_length); 
	
	
	remote.sin_family = AF_INET;
	remote.sin_port = htons(80); 
	
	
	connection = connect(sock, (struct sockaddr*)&remote, sizeof(struct sockaddr));
	if(connection < 0)
	{
		printf("Error -- Connection\n");
		exit(1);
	}
	
	
	char* template = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: HTMLGET 1.0\r\n\r\n";
	if(page[0]=='/')
		page++;
	int queryLen = strlen(template) + strlen(host) + strlen(page) - 3;

	char query[queryLen+1];
	
	sprintf(query, template, page, host);

	int sent = 0, bytesSent=0;
	while(sent < strlen(query))
	{
		bytesSent += send(sock, query + sent, strlen(query)-sent, 0); 
		if(bytesSent<0)
		{	
			printf("Error -- cant send query\n");
			exit(1);
		}
		sent += bytesSent;
	}
	
	bzero(buffer, sizeof(buffer));

	int recvBytes = 0, insideBody = 0;
	char* start, *end;
	printf("Without sparsing..........\n\n\n");
	while(recvBytes = recv(sock, buffer, sizeof(buffer), 0) > 0)
	{
		printf("%s\n",buffer);

		if(start = strstr(buffer, "<body"))
		{
			insideBody = 1;
			if(end = strstr(buffer, "</body>"))
			{
				
				printf("With sparsing..........\n\n\n");
				while(start < end) /
				{
					printf("%c", start[0]);
					start++;
				}
				insideBody = 0;
				
			}
			else 
				
				printf("%s", start);
		}
		
		else
		{
			
			if(insideBody)
			{
				if(end = strstr(buffer, "</body>"))
				{
					while(end >= 0)
					{
						printf("%c", end[0]);
						end--;
					}
					insideBody = 0;
				}
				else 
					printf("%s", buffer);
			}	
		}
		
	
		bzero(buffer, sizeof(buffer));
	}

}

