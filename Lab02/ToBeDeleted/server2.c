#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
using namespace std;

void doprocessing(int sd)
{   
	char buffer[1025];
                read( sd , buffer, 1024);// read the message

                // Generating the appropriate Messages to be sent later

                string ok="",bye,list="OK ";

                bye="Goodbye "+ok+":";// Goodbye message

                ok="OK "+ok+":"; // OK message
                
                if (buffer[0]== 'B'&&buffer[1]=='y'&&buffer[2]=='e'&&buffer[3]=='\n') // respond to Bye
                {
                    write(sd,bye.c_str(),30);  // Send the Goodbye message
                    close( sd );// Close the socket
                }
                else // reply with OK message
                    write(sd,ok.c_str(),30);
            }
int main(int argc , char *argv[])
{
    int main_socket , new_socket , client_socket[30],sd;// to store various socket descriptors,array of  descriptors

    int max_sd;// max socket fd needed for select fxn
    int pid;

    struct sockaddr_in cli_addr,serv_addr,cli_addr_array[30]; // address structs to store serv, new client, array for connected clients

    socklen_t addrlen=sizeof(cli_addr); //to store size of cli_addr

    char buffer[1025];  //data buffer of 1K
    
    //set of socket descriptors
    fd_set readfds;

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
    

    while(1)// Main Program Loop
    {   

    	  new_socket = accept(main_socket, (struct sockaddr *)&cli_addr, &addrlen);/// accept new connection
/* Create child process */
	      pid = fork();
			
	      if (pid < 0) {
	         perror("ERROR on fork");
	         exit(1);
	      }
	      
	      if (pid == 0) {
	         /* This is the client process */
	         close(main_socket);
	         doprocessing(new_socket);
	         return 0;
	      }
	      else {
	         close(new_socket);
	      }
      
        
    }
    
    return 0;
}
