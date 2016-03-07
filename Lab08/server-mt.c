#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <queue>
#include <pthread.h>
#include <signal.h>
using namespace std;
static int count = 0;

pthread_mutex_t mtx;
pthread_cond_t can_accept_conn;
pthread_cond_t can_process_conn;

int NUM_WORKERS = 10;
int MAX_QUEUE_SIZE = 10;
int PORT_NUM = 5000;
void *handleReq(void * a);
void sigpipe_handler(int a)
{
    handleReq(NULL);
}
queue<int> req;

// void* printer(void* sock1)
// {
// 	while(1)
// 	{
// 		sleep(1);
// 	}
// }

void* handleReq(void* sock1) 
{
	while(1)
	{
		pthread_mutex_lock(&mtx);
		while(!(req.size() > 0))
		{
			pthread_cond_wait(&can_process_conn, &mtx);
		}
		int sock = req.front();
		req.pop();
		pthread_cond_signal(&can_accept_conn);
		pthread_mutex_unlock(&mtx);


		int n;
		char buffer[512];
		bzero(buffer,512);

		//get filename to fetch
		n = read(sock,buffer,255);
		 
		if (n < 0) 
		{
			fprintf(stderr, "ERROR reading from socket");
			continue;
		}
		string req(buffer);
		//strip get and /n
		// printf("Req %s", req.c_str());
		req=req.erase(0,4);
		count ++;
		printf("%s %d\n",req.c_str(), count);
		//Fetch File from disk and send

		FILE *fp = fopen(req.c_str(),"r");
		if(fp==NULL)
		{
			printf("File open error");
			return NULL;   
		}   

		/* Read data from file and send it */
		while(1)
		{
			/* First read file in chunks of 256 bytes */
			bzero(buffer,512);
			int nread = fread(buffer,1,512,fp);      

			/* If read was success, send data. */
			if(nread > 0)
				write(sock, buffer, nread);

			// file finished
			if (nread < 512)
			{
				if (ferror(fp))
					printf("Error reading\n");
				break;
			}
		}
		//close file
exit1:
		fclose(fp);
		close(sock);
	}
}

int main( int argc, char *argv[] ) 
{
	int main_socket , new_socket,sd;// to store various socket descriptors
	int pid; // pid of forked childs
	char buffer[1025];  //data buffer of 1K

	if (argc !=4) 
	{ // Checking if correct number of arguments supplied
		fprintf(stderr,"Wrong No. of arguments supplied\n");
		return 1;
	}
	signal(SIGPIPE,sigpipe_handler);
	PORT_NUM = stoi(argv[1]);
	NUM_WORKERS = stoi(argv[2]);
	MAX_QUEUE_SIZE = stoi(argv[3]);

	cout << PORT_NUM << " " << NUM_WORKERS << " " << MAX_QUEUE_SIZE << endl;

	// signal(SIGCHLD,chld_reap_handler); //Initialze signal for handling child reaping
	struct sockaddr_in cli_addr,serv_addr; // address structs to store serv, new client, array for connected clients
	socklen_t addrlen=sizeof(cli_addr); //to store size of cli_addr   

	//Open the Main socket
	main_socket = socket(AF_INET , SOCK_STREAM , 0);
	
	//Done so that socket/port maybe resued after closing server without having to wait for timeout
	int opt = 1;
	setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	
	//Initiliasing serv_addr fields
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT_NUM);
	
	if (bind(main_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)// Bind the main socket and chk for binding errors
	{
		fprintf(stderr,"ERROR on socket Binding\n");
		return 2;
	}
	


	pthread_t thread[NUM_WORKERS];
	// pthread_t thread1;
	int i = 0;
	// pthread_create(&thread1, NULL, printer, NULL);
	while(i < NUM_WORKERS)
	{
		pthread_create(&thread[i], NULL, handleReq, NULL);
		i++;
	}
	listen(main_socket, 5);// Listen on main socket

	while (1) 
	{
		new_socket = accept(main_socket, (struct sockaddr *) &cli_addr, &addrlen);
	
		
		pthread_mutex_lock(&mtx);
		while(!(MAX_QUEUE_SIZE == 0 || req.size() < MAX_QUEUE_SIZE))
		{
			pthread_cond_wait(&can_accept_conn, &mtx);
		}
		req.push(new_socket);
		pthread_cond_signal(&can_process_conn);
		pthread_mutex_unlock(&mtx);
	}
}
