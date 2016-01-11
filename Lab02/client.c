#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
char **glob;
int temp = 0;
void fxn(void* a)
{
	printf("%d\n",temp );
	while(1)
	{	
		int sockfd, portno, n;
		struct sockaddr_in serv_addr;
		struct hostent *server;
		int bytesReceived = 0;
		char recvBuff[256];
		char buffer[256];
		portno = atoi(glob[2]);
		/* Create a socket point */
		sockfd = socket(AF_INET, SOCK_STREAM, 0);

		if (sockfd < 0) 
		{
			perror("ERROR opening socket");
			exit(1);
		}

		server = gethostbyname(glob[1]);

		if (server == NULL) 
		{
			fprintf(stderr,"ERROR, no such host\n");
			exit(0);
		}

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(portno);

		/* Now connect to the server */
		if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
		{
			perror("ERROR connecting");
			exit(1);
		}
 
		/* Now ask for a message from the user, this message
		* will be read by server
		*/
		 
		/* Send message to the server */

		char str[50] = "get files/foo1.txt";
		//char num= atoi(temp);
		//strcat(str, num);

		n = write(sockfd, str,50);
		if (n < 0) 
		{
			perror("ERROR writing to socket");
			exit(1);
		}
		 
		/* Now read file from server */
		FILE *fp;
		/*
		fp = fopen("sample_file.txt", "ab"); 
		if(NULL == fp)
		{
				printf("Error opening file");
				return;
		}   
		/* Receive data in chunks of 256 bytes */
		while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
		{
			 // printf("Bytes received %d\n",bytesReceived);    
				// recvBuff[n] = 0;
				//fwrite(recvBuff, 1,bytesReceived,fp);
			 // printf("%s \n", recvBuff);
		}

		if(bytesReceived < 0)
		{
				printf("\n Read Error \n");
		}
		close(sockfd);
		return;
	}
}
int main(int argc, char *argv[]) 
{


	if (argc < 3) 
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	
	glob=argv;
	int i=0;
	pthread_t thread[100];
	while(i<100)
	{
		void* a;
		pthread_create( &thread[i], NULL, fxn, a);
		i++;
		temp++;
	}
	i=0;
	while(i<100)
	{
		pthread_join( thread[i], NULL);i++;
	}
}
