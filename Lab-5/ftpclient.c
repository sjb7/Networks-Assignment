#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <unistd.h>
#include <dirent.h> 

#define MAX_SIZE 20000
int main()
{
	int sockfd;
	struct sockaddr_in my_addr;
	struct sockaddr_in dest_addr;
	FILE *fp;
	DIR *d;
	struct dirent *dir;

	char message[30];
	char buffer[MAX_SIZE];
	char ls_reply[MAX_SIZE];
	char line[100];
	char mput_send[MAX_SIZE];
	
	int bytes_sent,bytes_recv;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		printf("Could not create socket");

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(9996);
	dest_addr.sin_addr.s_addr = INADDR_ANY;
	
	if(connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
		printf("Connection Error\n");

		printf("Connection Established\n");

	while(1)
	{
		printf("Commands : 1)ls\t2)get\t3)put\t4)mget\t5)mput\t6)cd\t7)del\n");
		printf("Enter the message:");
		gets(message);

		if(send(sockfd,(void *)message,strlen(message),0)  < 0)
		{
			printf("Error in sending the message\n");
			exit(0);
		}
		printf("Message sent ...\n\n");

		if(strcmp(message,"bye") == 0)
		{
			break;
		}
		else
		{
			if(strcmp(message,"ls") == 0)									
			{
				memset(buffer,'\0',MAX_SIZE);
				if(recv(sockfd, buffer, MAX_SIZE, 0) < 0)
					printf("Receive Failed");

				memset(ls_reply, '\0', MAX_SIZE);
			    memcpy(ls_reply, buffer,MAX_SIZE);
				printf("%s", buffer);
			}
			else
			{
				if(strstr(message,"get ") != NULL)						
				{
					memset(buffer, '\0', MAX_SIZE);
					printf("*********get %s*********\n",message+4);
					fp = fopen(message+4,"w");
					if(fp == NULL)
						printf("Could not open the file\n");
				
					recv(sockfd, (void *)buffer, MAX_SIZE, 0);
						fprintf(fp,"%s", buffer);

					printf("*********123*********\n");

					fclose(fp);

				}
				else
				{
					if(strstr(message,"mget*") != NULL)				
					{
						char *tok;

				        printf("ls_reply %s\n",ls_reply);
				       	tok = strtok(ls_reply,"\n");
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
					
							if(recv(sockfd, (void *)buffer, MAX_SIZE, 0) > 0)
								fprintf(fp, "%s", buffer);
							
							tok = strtok(NULL,"\n");
							printf("\n--tok2 %s \n",tok);
							fclose(fp);
				
		        		}

					}
					else
					{
						if(strstr(message,"mget*.txt") != NULL)			
						{
							char * tok;
					        tok = strtok(ls_reply,"\n");
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
							
									if(recv(sockfd, (void *)buffer, MAX_SIZE, 0) > 0)
										fprintf(fp, "%s", buffer);
							
									tok = strtok(NULL,"\n");
									printf("\n--tok2 %s \n",tok);
                    				fclose(fp);
				
		        			}

						}
						else
						{
							if(strstr(message,"put ") != NULL)		
							{
								memset(buffer, '\0', MAX_SIZE);
								fp=fopen(message + 4,"r");
					                                if(fp == NULL)
								printf("Could not open the file\n");

								while(fgets(line, sizeof line, fp) != NULL)
									strcat(buffer, line);

								if((send(sockfd, (void *)buffer, strlen(buffer), 0)) < 0)
									printf("Sending Error\n");

								fclose(fp);
								
							}
							else
							{
								if(strstr(message,"mput*") != NULL)		
								{
									printf("-------1----------\n");
									memset(buffer, '\0', MAX_SIZE);
									
									char * tok;
							        printf("ls_reply_client_side %s\n",mput_send);

							        tok = strtok(mput_send,"\n");
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
							                	printf("***** tok = %s \n",tok);
							                    continue;
							                        
							                }
	                
											fp=fopen(tok,"r");
											if(fp == NULL)
												printf("Could not open the file\n");
											
											while(fgets(line, sizeof line, fp) != NULL)
												strcat(buffer, line);
												
							                if((send(sockfd, (void *)buffer, strlen(buffer), 0)) < 0)
													printf("Sending Error\n");
							                                					
											
											tok = strtok(NULL,"\n");
										    printf("\n-tok %s \n",tok);
											fclose(fp);
								
									}

								}
								else
								{
									if(strstr(message,"mput*.txt") != NULL)		
									{
										memset(buffer, '\0', MAX_SIZE);
										
										char * tok;
								        printf("ls_reply_client_side %s\n",mput_send);

								        tok = strtok(mput_send,"\n");
						       		    printf("\ntok %s \n",tok);

								        while(tok != NULL)
										{
								            memset(buffer, '\0', MAX_SIZE);
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
													
								            if((send(sockfd, (void *)buffer, strlen(buffer), 0)) < 0)
											    	printf("Sending Error\n");
								                                					
											tok = strtok(NULL,"\n");
											printf("\n- tok %s \n",tok);
											fclose(fp);	
												
										}
										        
									
									}
									else
									{
										if(strstr(message,"cd ") != NULL)		
										{
											memset(buffer, '\0', MAX_SIZE);
											if(recv(sockfd,(void *)buffer,MAX_SIZE,0) < 0)
											{
												printf("Error in getting the acknowledge\n");
												
											}

										}
										else if(strcmp(message,"lsc") == 0)		
										{

											memset(buffer, '\0', MAX_SIZE);
											d = opendir("."); 				if(d)
											{
	    										while ((dir = readdir(d)) != NULL)  
	    										{
													strcat(buffer, dir->d_name);
													strcat(buffer, "\n");
	    										}
	    										closedir(d); 
	 										}
	 										memcpy(mput_send, buffer, MAX_SIZE);
	 										printf("lsc command = %s",mput_send);

											if(send(sockfd, (void *)buffer, strlen(buffer), 0) < 0)
													printf("Sending Error\n");
										}
									}

								}

								}

							}
						}
					}
				}
			}
		}


}
