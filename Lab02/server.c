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
using namespace std;

static void
  sigchld_handler(int signo) {
      pid_t PID;
      do  {
          PID = waitpid(-1,0,WNOHANG);
      } while ( PID != -1 );

     /* Re-instate handler */
      signal(SIGCHLD,sigchld_handler);
  }
void childprocessing (int sock) {
   int n;
   char buffer[256];
   bzero(buffer,256);

//get filename to fetch
   n = read(sock,buffer,255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   string req(buffer);
//strip get and /n
   req=req.erase(0,4);
   //req=req.substr(0,req.length()-1);
   printf("%s\n",req.c_str());
   //Fetch File from disk and send

        FILE *fp = fopen(req.c_str(),"r");
        if(fp==NULL)
        {
            printf("File open error");
            return;   
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            bzero(buffer,256);
            int nread = fread(buffer,1,256,fp);      

            /* If read was success, send data. */
            if(nread > 0)
            {
                write(sock, buffer, nread);
            }

	// file finished
            if (nread < 256)
            {
                if (ferror(fp))
                    printf("Error reading\n");
		break;
         	}
	}
	fclose(fp);
}

int main( int argc, char *argv[] ) {
    int main_socket , new_socket,sd;// to store various socket descriptors,array of  descriptors


    int pid;

    char buffer[1025];  //data buffer of 1K
    

    if (argc !=2) { // Checking if correct number of arguments supplied
        fprintf(stderr,"Wrong No. of arguments supplied\n");
        return 1;
    }

    char* p;
    int portno = strtol(argv[1], &p, 10);//converting supplied port no to int and storing

    if (*p) {// conversion failed because the input wasn't a number

        fprintf(stderr,"Invalid Port No.\n");
        return 1;
    }
    signal(SIGCHLD,sigchld_handler);
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
    serv_addr.sin_port = htons(portno);
    
    if (bind(main_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)// Bind the main socket and chk for binding errors
    {
        fprintf(stderr,"ERROR on socket Binding\n");
        return 2;
    }
    
    listen(main_socket, 5);// Listen on main socket

   while (1) {
      new_socket = accept(main_socket, (struct sockaddr *) &cli_addr, &addrlen);
		
      if (new_socket < 0) {
         perror("ERROR on accept");
         exit(1);
      }
      
      /* Create child process */
      pid = fork();
		
      if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
      
      if (pid == 0) {
         /* This is the child process */
         close(main_socket);
         childprocessing(new_socket);
         exit(0);
      }
      else {
         close(new_socket);
      }
		
   } /* end of while */
}
