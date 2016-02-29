#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#define NUM_FILES 500

int count = 0;
int k;
int locked=0;
void fxn(void* a)
{	
	for(int i=0;i<k;i++){
		while(locked) //do nothing; busy wait
			locked = 1; //set lock
		count++; //increment
		locked = 0; //release lock
	}
}
int main(int argc, char *argv[]) 
{	
	srand(time(NULL));
	int numProc = atoi(argv[1]);
	int i=0;
	k= atoi(argv[2]);
	struct timeval tim;
	gettimeofday(&tim, NULL);
	double reqStart = tim.tv_sec + (tim.tv_usec/1000000.0);
	pthread_t thread[1000];
	while(i<numProc)
	{

		pthread_create( &thread[i], NULL, (void*)fxn, NULL);
		i++;
	}
	i=0;
	while(i<numProc)
	{
		pthread_join( thread[i], NULL);i++;
	}

		gettimeofday(&tim, NULL);
		double reqEnd = tim.tv_sec + (tim.tv_usec/1000000.0);
	printf("time%f \n",reqEnd-reqStart);	
	printf("count %d\n", count);	
}