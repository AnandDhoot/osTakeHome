#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include "make-tokens.h"
#include <vector>

using namespace std;

int main(void)
{
		char  line[MAX_INPUT_SIZE];            
		char  **tokensC;              
		int i;

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
				printf("Server command\n");
			}
			else if(tokens[0].compare("getfl") == 0)
			{
				printf("Get file command\n");
			}
			else if(tokens[0].compare("getsq") == 0)
			{
				printf("Get files sequentially command\n");
			}
			else if(tokens[0].compare("getpl") == 0)
			{
				printf("Get files parallely command\n");
			}
			else if(tokens[0].compare("getbg") == 0)
			{
				printf("Get file in background command\n");
			}
			else if(tokens[0].compare("ls") == 0)
			{
				printf("ls command\n");
			}
			else if(tokens[0].compare("cat") == 0)
			{
				printf("Cat command\n");
			}
			else if(tokens[0].compare("echo") == 0)
			{
				printf("Echo command\n");
			}
			else if(tokens[0].compare("exit") == 0)
			{
				printf("Exit command\n");
			}
			else 
			{
				fprintf(stderr, "Incorrect command.\n");
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