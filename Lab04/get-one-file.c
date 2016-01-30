#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char *argv[]) {

   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
       int bytesReceived = 0;
    char recvBuff[512];
   
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
	
   portno = atoi(argv[3]);
   
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
	
   server = gethostbyname(argv[2]);
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
   
   /* Now ask for a message from the user, this message
      * will be read by server
   */
	

   /* Send message to the server */
   n = write(sockfd, argv[1], strlen(argv[1]));
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   
   /* Now read file from server */
    /* Receive data in chunks of 256 bytes */
    while((bytesReceived = read(sockfd, recvBuff, 512)) > 0)
    {
       // printf("Bytes received %d\n",bytesReceived);    
        // recvBuff[n] = 0;
        if(argv[4][0]=='d')
        	printf("%.*s",bytesReceived,recvBuff);

    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }
   return 0;
}
