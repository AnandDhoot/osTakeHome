#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#define NUM_FILES 500

char **glob;
int temp = 0;
long reqFulfilled[100];
double responseTime[100];
void fxn(void* a)
{	
	int thread_id= *(int*)a; //id of the current thread
	reqFulfilled[thread_id]=0;
	printf("%d\n",thread_id);
	
	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1 = tim.tv_sec + (tim.tv_usec/1000000.0);

	while(1)
	{	
		int r = rand() % NUM_FILES; // gen int to fetch random file

		int sockfd, portno, n;
		struct sockaddr_in serv_addr;
		struct hostent *server;
		
		char recBuff[512];
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

		 
		int random = 0;
		if(strcmp("random", glob[6]) == 0)
			random = 1;

		char str[50] = "get files/foo1.txt";

		if(random == 1)
			sprintf(str, "get files/foo%d.txt", r);


		struct timeval tim;
		gettimeofday(&tim, NULL);
		double reqStart = tim.tv_sec + (tim.tv_usec/1000000.0);

		n = write(sockfd, str,50);
		if (n < 0) 
		{
			perror("ERROR writing to socket");
			exit(1);
		}
		 
		/* Now read file from server */

		//FILE *fp;
		/*
		fp = fopen("sample_file.txt", "ab"); 
		if(NULL == fp)
		{
				printf("Error opening file");
				return;
		}
   
		/* Receive data in chunks of 512 bytes */
		int bytesReceived = 0;
		while((bytesReceived = read(sockfd, recBuff, 512)) > 0)
		{
			 // printf("Bytes received %d\n",bytesReceived);    
				// recvBuff[n] = 0;
				//fwrite(recvBuff, 1,bytesReceived,fp);
			 // printf("%s \n", recvBuff);
			//reqFulfilled[thread_id]+=bytesReceived;
		}

		if(bytesReceived < 0)
		{
			printf("\n Read Error \n");
		}
		close(sockfd);

		// struct timeval tim;
		gettimeofday(&tim, NULL);
		double reqEnd = tim.tv_sec + (tim.tv_usec/1000000.0);

		reqFulfilled[thread_id]++;
		responseTime[thread_id] += (reqEnd - reqStart);

		// struct timeval tim;
		gettimeofday(&tim, NULL);
		double t2 = tim.tv_sec + (tim.tv_usec/1000000.0);

		printf("%d, %f\n", atoi(glob[4]), t2-t1);
		if((t2-t1) > atoi(glob[4]))
			break;

		sleep(atoi(glob[5]));
	}

	responseTime[thread_id] /= reqFulfilled[thread_id];
	return;
}
int main(int argc, char *argv[]) 
{	
	srand(time(NULL));
	int numProc = atoi(argv[3]);

	if (argc < 3) 
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	
	glob=argv;
	int i=0;
	pthread_t thread[numProc];
	while(i<numProc)
	{
		int *a = malloc(sizeof(*a));
		*a=i;
		pthread_create( &thread[i], NULL, fxn, (void*)a);
		i++;
		temp++;
	}
	i=0;
	while(i<numProc)
	{
		pthread_join( thread[i], NULL);i++;
	}
	i=0;
	float totalData=0.0;
	float totalResTime=0.0;
	//Stats Calculations
	while(i<numProc)
	{
		totalData+=reqFulfilled[i];
		totalResTime += responseTime[i];
		i++;
	}

	totalResTime /= (atoi(argv[3]));

	printf("throughput = %f (req/s)\n",totalData/atoi(argv[4]));	
	printf("average response time = %f sec\n", totalResTime);	
}
