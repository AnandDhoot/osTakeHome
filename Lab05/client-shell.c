#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include "make-tokens.h"
#include <vector>
#include <algorithm>

using namespace std;

string IP = "127.0.0.1";
string PORT = "5000";

vector<int> backPIDs;

void sig_segfault(int w)
{
	// exit(0);
}

void sig_chld_handler(int x)
{
	printf("File download completed in background process\n");
	pid_t id;
	do  
	{
		id = waitpid(-1,0,WNOHANG);
		printf("%d\n", id);
		if(id > 0)
			backPIDs.erase(find(backPIDs.begin(), backPIDs.end(), id));.
	}
	while ( id > 0 );
	signal(SIGCHLD, sig_chld_handler);
}

void forground(vector<string> tokens, char* args[])
{
	string ex ="/bin/";
	ex+=tokens[0];
	if( access( ex.c_str(), F_OK ) != -1 )
	{
		execvp(ex.c_str(),args);
	}
	else if(tokens[0] == "getfl")
	{
		if(tokens.size() == 2)
		{
			execl("../Lab04/get-one-file-sig", "get-one-file-sig", 
				tokens[1].c_str(), IP.c_str(), PORT.c_str(), "display", 0);
		}
		else if(tokens.size() == 4 && tokens[2] == ">")
		{
			if(freopen(tokens[3].c_str(), "w+", stdout) < 0)
			{
				fprintf(stderr, "Error opening file to write");
				exit(1);
			}
			execl("../Lab04/get-one-file-sig", "get-one-file-sig", 
				tokens[1].c_str(), IP.c_str(), PORT.c_str(), "display", 0);
			fclose(stdout);
		}
		else if(tokens.size() >= 4 && tokens[2] == "|")
		{
			int pipefd[2];
			pipe(pipefd);
			char*  a[10];
			int i;
			for(i=3;i<tokens.size();i++)
				a[i-3]=strdup(tokens[i].c_str());
			a[i-3]= NULL;

			int pid = fork();

			if (pid == 0)
			{
				dup2(pipefd[0], 0);
				close(pipefd[1]);
				
				string s = "/bin/";
  				s+=tokens[3];

				if(access( s.c_str(), F_OK ) != -1)
				{
					execvp(s.c_str(),a);
				}
				else
				{
					printf("%s\n","Wrong command afer pipe" );
					exit(0);
				}
			}
			else
			{
				int x= fork();
				if(x==0)
				{
					dup2(pipefd[1], 1);
					close(pipefd[0]);

					execl("../Lab04/get-one-file-sig", "get-one-file-sig", 
						tokens[1].c_str(), IP.c_str(), PORT.c_str(), "display", 0);
				}
				else
				{
					close(pipefd[0]);
					close(pipefd[1]);

					for(int k=0;k<=i-3;k++)
						free(a[k]);

					waitpid(x, NULL, 0);
					waitpid(pid, NULL, 0);
				}
			}
		}
		else
			fprintf(stderr, "Incorrect number of arguments with getfl\n");
	}
	else if(tokens[0] == "getsq")
	{
		if(tokens.size() <= 1)
			fprintf(stderr, "Atleast one file name must be provided with getsq\n");
		for(int i=1; i<tokens.size(); i++)
		{
			int pid1 = fork();
			if(pid1 == 0)
			{
				execl("../Lab04/get-one-file-sig", "get-one-file-sig", 
				tokens[i].c_str(), IP.c_str(), PORT.c_str(), "nodisplay", 0);
			}
			else
			{
				waitpid(pid1, NULL, 0);
			}
		}
	}
	else if(tokens[0] == "getpl")
	{	
		if(tokens.size() <= 1)
		{
			fprintf(stderr, "Atleast one file name must be provided with getpl\n");
			exit(1);	
		}

		vector<int> pidArr(tokens.size(), 0);
		for(int i=1; i<tokens.size(); i++)
		{
			pidArr[i] = fork();
			if(pidArr[i] == 0)
			{
				execl("../Lab04/get-one-file-sig", "get-one-file-sig", 
					tokens[i].c_str(), IP.c_str(), PORT.c_str(), "display", 0);
			}
		}
		for (int i = 1; i < tokens.size(); i++)
		{
			waitpid(pidArr[i], NULL, 0);
		}
	}
	else
	{
		printf("%s\n",tokens[0].c_str() );
		fprintf(stderr, "Incorrect command.\n");
	}
	exit(1);
}

int main(void)
{
	char line[MAX_INPUT_SIZE];            
	char **tokensC;              
	int i;
	signal(SIGINT, sig_segfault);
	signal(SIGCHLD,sig_chld_handler);
	while (1) 
	{
		//sigprocmask(SIG_BLOCK, &signal_set, NULL);
		printf("Hello> ");     
		bzero(line, MAX_INPUT_SIZE);
		gets(line);
		line[strlen(line)] = '\n'; //terminate with new line
		tokensC = tokenize(line);

		vector<string> tokens;
		for(i=0;tokensC[i]!=NULL;i++)
		{
			string str = (string) tokensC[i];
			tokens.push_back(str);
		}
		
		char*  a[64];
		for(i=0;i<tokens.size();i++)
		a[i]=strdup(tokens[i].c_str());
		a[i]= NULL;

		if(tokens[0].compare("cd") == 0)
		{
			if(tokens.size() != 2)
				fprintf(stderr, "Exactly one argument expected with cd\n");
			else
			{
				int res = chdir(tokens[1].c_str());
				if(res != 0)
					fprintf(stderr, "cd command could not be executed\n");
				else
				{
					char token[MAX_TOKEN_SIZE];
					getcwd(token, sizeof(token));
					fprintf(stdout, "Changed to the directory: %s\n", token);
				}
			}
		}
		else if(tokens[0].compare("server") == 0)
		{
			if(tokens.size() != 3)
				fprintf(stderr, "Exactly two arguments expected with server\n");
			else
			{
				IP = tokens[1];
				PORT = tokens[2];
			}
		}
		else if(tokens[0] == "getbg")
		{
			if(tokens.size() != 2)
			{
				fprintf(stderr, "Exactly two arguments expected with getbg\n");
				exit(1);	
			}
			int pid = fork();

			if(pid == 0)
			{

				execl("../Lab04/get-one-file-sig", "get-one-file-sig", 
					tokens[1].c_str(), IP.c_str(), PORT.c_str(), "nodisplay", 0);
			}
			else
			{
				setpgid(pid, 0);
				backPIDs.push_back(pid);
			}
		}
		else if(tokens[0].compare("exit") == 0)
		{
			if(tokens.size() != 1)
				fprintf(stderr, "No arguments expected with exit\n");
			else
			{
				for(int i=0; i<backPIDs.size(); i++)
					kill(backPIDs[i], SIGINT);

				break;
			}
		}
		else
		{
			signal(SIGCHLD,sig_segfault);
			int pid = fork();
			if(pid == 0)
			{
				signal(SIGINT,SIG_DFL);
				forground(tokens,a);
			}
			else
			{
				waitpid(pid, NULL, 0);
				signal(SIGCHLD,sig_chld_handler);
			}
		}
		
		// Freeing the allocated memory
		for(int k=0;k<=tokens.size();k++)
					free(a[k]);
		for(i=0;tokensC[i]!=NULL;i++)
		{
			free(tokensC[i]);
		}
		free(tokensC); 
	}
		 
	return 0;
}