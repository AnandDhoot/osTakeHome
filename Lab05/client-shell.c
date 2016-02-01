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

using namespace std;

string IP = "127.0.0.1";
string PORT = "5000";

void sig_segfault(int w)
{
		// exit(0);
}

void forker(vector<string> tokens)
{
	if(tokens[0] == "getfl")
	{
		if(tokens.size() == 2)
		{
			execl("/users/ug13/ananddhoot/Desktop/333/Lab04/get-one-file", "get-one-file", 
				tokens[1].c_str(), IP.c_str(), PORT.c_str(), "display", 0);
		}
		else if(tokens.size() == 4 && tokens[2] == ">")
		{
			if(freopen(tokens[3].c_str(), "w+", stdout) < 0)
			{
				fprintf(stderr, "Error opening file to write");
				exit(1);
			}
			execl("/users/ug13/ananddhoot/Desktop/333/Lab04/get-one-file", "get-one-file", 
				tokens[1].c_str(), IP.c_str(), PORT.c_str(), "display", 0);
			fclose(stdout);
		}
		else if(tokens.size() == 4 && tokens[2] == "|")
		{
			printf("Pipe\n");
		}
		else
			fprintf(stderr, "Incorrect number of arguments with getfl\n");
	}
	else if(tokens[0] == "getsq")
	{
		// if(tokens.size() <= 1)
		// 	fprintf(stderr, "Atleast one file name must be provided with getsq\n");
		// for(int i=1; i<tokens.size(); i++)
		// {
		// 	execl("/users/ug13/ananddhoot/Desktop/333/Lab04/get-one-file", "get-one-file", 
		// 		tokens[i].c_str(), IP.c_str(), PORT.c_str(), "display", 0);
		// }
		printf("Getsq command\n");
	}
	else if(tokens[0] == "getpl")
	{
		printf("Get files parallely command\n");
	}
	else if(tokens[0] == "getbg")
	{
		printf("Get file in background command\n");
	}
	else if(tokens[0] == "ls")
	{
		execl("/bin/ls", "ls", 0);
	}
	else if(tokens[0] == "cat")
	{
		execl("/bin/cat", "cat", tokens[1].c_str(), 0);
	}
	else if(tokens[0] == "echo")
	{
		printf("Echo command\n");
	}
	else
	{
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

	while (1) 
	{
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
		else if(tokens[0].compare("exit") == 0)
		{
			if(tokens.size() != 1)
				fprintf(stderr, "No arguments expected with exit\n");
			else
				break;
		}
		else
		{
			int pid = fork();
			if(pid == 0)
			{
				forker(tokens);
			}
			else
			{
				waitpid(pid, NULL, 0);
			}
		}
		 
		// Freeing the allocated memory	
		for(i=0;tokensC[i]!=NULL;i++)
		{
 			free(tokensC[i]);
		}
		free(tokensC);
	}
		 
	return 0;
}