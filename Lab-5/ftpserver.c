#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_SIZE 20000

char ls_buffer[MAX_SIZE];

int main()
{
	int sd;
	int newfd;

	struct sockaddr_in myaddr;
	struct sockaddr_in clientaddr;
	int sin_size;

	char buffer[MAX_SIZE];
	char line[1000];
	char message[200];
	char ls_reply[MAX_SIZE];
	char lsc_reply[MAX_SIZE];

	FILE *fp;
	DIR *d;
	struct dirent *dir;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd == -1)
		printf("Could not create socket");
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(9996);
	myaddr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sd, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0)
		printf("Binding failed\n");

	printf("Bind done\n");

	if(listen(sd, 5) < 0)
		printf("Listening failed\n");

	printf("Listening ...\n");
	sin_size = sizeof(struct sockaddr_in);

	newfd = accept(sd, (struct sockaddr *) &clientaddr, (socklen_t *) &sin_size);
	if(newfd < 0)
		printf("Connection Failed\n");

	printf("Connection established\n");			

	memset(message, '\0', 200);

	while((recv(newfd, (void *)message, 200, 0)) > 0)
	{
		memset(buffer, '\0', MAX_SIZE);
		memset(line, '\0', 100);

		

		if(strcmp(message, "bye") == 0)				
		{
			close(newfd);
			break;
		}
		else if(strcmp(message, "ls") == 0)			
		{
			memset(buffer, '\0', MAX_SIZE);
			d = opendir("."); 
			if(d)
			{
	    			while ((dir = readdir(d)) != NULL) 
	    			{
					strcat(buffer, dir->d_name);
					strcat(buffer, "\n");
	    			}
	    			closedir(d); 
	 		}
	 		memcpy(ls_reply, buffer, MAX_SIZE);

			if(send(newfd, (void *)buffer, strlen(buffer), 0) < 0)
				printf("Sending Error\n");

		}
		else if(strcmp(message, "mget*") == 0)			
		{
		        printf("\nmget*\n");
		        char * tok;
		        tok = strtok(ls_reply,"\n");
		        printf("\ntok2 %s \n",tok);
		        while(tok != NULL)
		        {
		        		memset(buffer, '\0', MAX_SIZE);
		                if((strcmp(tok,"..")==0) )
		                {
               				tok = strtok(NULL,"\n");
		                    continue;
		                }
		                if((strcmp(tok,"a.out")==0) )
		                {
               				tok = strtok(NULL,"\n");
		                    continue;
		                }
		                if(( strcmp(tok,".")==0 ) )
		                {
		                	tok = strtok(NULL,"\n");
		                    continue;
		                }
		               
		                fp=fopen(tok,"r");
		                if(fp == NULL)
							printf("Could not open the file\n");

						while(fgets(line, sizeof line, fp) != NULL)
							strcat(buffer, line);
					
                        if((send(newfd, (void *)buffer, strlen(buffer), 0)) < 0)
							printf("Sending Error\n");
                                					
				
						tok = strtok(NULL,"\n");
			        	printf("\n-tok2 %s \n",tok);
						fclose(fp);
				
		        }
        }else if (strcmp(message,"mget*.txt") == 0)		
      		  {
                    printf("\nmget*.txt\n");    
			        char * tok;
			        tok = strtok(ls_reply,"\n");
			        while(tok != NULL)
			        {
			            memset(buffer,'\0',MAX_SIZE);
			            memset(line,'\0',100);
			            if(strstr(tok,".txt") == NULL )
			            {
	                        tok = strtok(NULL,"\n");
	                        continue;
			            }

			            fp=fopen(tok,"r");
			            if(fp == NULL)
							printf("Could not open the file\n");

						while(fgets(line, sizeof line, fp) != NULL)
							strcat(buffer, line);
						
	                    if((send(newfd, (void *)buffer, strlen(buffer), 0)) < 0)
							printf("Sending Error\n");
	                                					
						tok = strtok(NULL,"\n");
				        printf("\n- tok2 %s \n",tok);
					
						fclose(fp);
			        }

	                
	        }else if(strstr(message,"get ") != NULL)				
				  {
				  	printf("\n\nFile to be sent = %s\n\n",message+4);
					fp = fopen(message+4, "r");
					if(fp == NULL)
						printf("Could not open the file\n");

					memset(buffer,'\0',MAX_SIZE);
			        memset(line,'\0',100);
					
					while(fgets(line, sizeof line, fp) != NULL)
						strcat(buffer, line);

					if((send(newfd, (void *)buffer, strlen(buffer), 0)) < 0)
							printf("Sending Error\n");
					puts(buffer);
					printf("\n\n-----------------------\n\n");

					fclose(fp);
				}
				else if(strstr(message,"del") != NULL)				
				{
					if(remove(message+4) != 0)
						printf("Error deleting file\n");
				}else if(strstr(message,"put ") != NULL)			
				{
					memset(buffer, '\0', MAX_SIZE);
					fp = fopen(message+4, "w");
					if(fp == NULL)
						printf("Could not open the file\n");
						
				        while(fgets(line, sizeof line, fp) != NULL)
			                        	strcat(buffer, line);
			                        	
					if(recv(newfd, (void *)buffer, MAX_SIZE, 0) > 0)
						fprintf(fp, "%s", buffer);

					fclose(fp);
				}
				else if (strcmp(message,"lsc") == 0)		
				{
					printf("lsc command\n");
					memset(buffer,'\0',MAX_SIZE);
					if(recv(newfd, buffer, MAX_SIZE, 0) < 0)
						printf("Receive Failed");

					memset(lsc_reply, '\0', MAX_SIZE);
			    	memcpy(lsc_reply, buffer,MAX_SIZE);
					printf("%s", buffer);

				}
				else if (strstr(message,"mput*") != NULL)		
				{
						char *tok;

				        printf("lsc_reply %s\n",lsc_reply);
				       	tok = strtok(lsc_reply,"\n");
				       	printf("\ntok %s \n",tok);

				       	while(tok != NULL)
		                { 	
							memset(buffer, '\0', MAX_SIZE);                
			                if((strcmp(tok,"..")==0) )
			                {
	               				tok = strtok(NULL,"\n");
			                        continue;
			                }
			                if((strcmp(tok,"a.out")==0) )
			                {
	               				tok = strtok(NULL,"\n");
			                        continue;
			                }
			                if(( strcmp(tok,".")==0 ) )
			                {
			                	tok = strtok(NULL,"\n");
			                	printf("tok = %s \n",tok);
			                    continue;
			                }
                     
			                fp = fopen(tok,"w");
			                if(fp == NULL)
							printf("Could not open the file\n");
					
							if(recv(newfd, (void *)buffer, MAX_SIZE, 0) > 0)
								fprintf(fp, "%s", buffer);
							
							tok = strtok(NULL,"\n");
							printf("\n--tok %s \n",tok);
							fclose(fp);
						}
				}
				else if(strstr(message,"mput*.txt") != NULL)		
				{
							char * tok;
					        tok = strtok(lsc_reply,"\n");
					        while(tok != NULL)
					        {
			        		        memset(buffer,'\0',MAX_SIZE);
					                if(strstr(tok,".txt") == NULL ) 
					                {
			                            tok = strtok(NULL,"\n");
			                            continue;
					                }
					                
					                fp=fopen(tok,"w");
					                if(fp == NULL)
									printf("Could not open the file\n");
							
									if(recv(newfd, (void *)buffer, MAX_SIZE, 0) > 0)
										fprintf(fp, "%s", buffer);
							
									tok = strtok(NULL,"\n");
									printf("\n--tok2 %s \n",tok);
									fclose(fp);				
		        			}
                    		
				}
				memset(message, '\0', 200);
		}

			close(sd);
			return 0;
}
