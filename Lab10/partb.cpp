#include <sys/mman.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sstream>
#include <sys/time.h>

using namespace std;

int main()
{
	string str = "free -m";
	int fd[25];
	for(int i=0; i<25; i++)
	{
		string name = "file" + to_string(i);
		fd[i] = open(name.c_str(), O_RDWR);
	}
	int pid = getpid();
	string cmmd = "pmap -x " + to_string(pid) + " | tail -n 1";

	system(cmmd.c_str());

	char* p[25];

	cout << "Mapping the files" << endl;
	for(int i=0; i<25; i++)
	{
		p[i] = (char*) mmap (0, 10000000, PROT_READ | PROT_WRITE, MAP_SHARED, fd[i], 0);
		if (p[i] == MAP_FAILED)
		        perror ("mmap");
	}

	system(cmmd.c_str());

	int fd1;
	char* data = "3";

	sync();
	fd1 = open("/proc/sys/vm/drop_caches", O_WRONLY);
	write(fd1, data, sizeof(char));
	close(fd1);

	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1 = tim.tv_sec + (tim.tv_usec/1000000.0);

	for(int i=0; i<25; i++)
	{
		char* rdSeek = p[i];
		int num = 0;
		while(num < 10000000)
		{
			char a = *(rdSeek + num);
			num += 1;
		}
	}

	gettimeofday(&tim, NULL);
	double t2 = tim.tv_sec + (tim.tv_usec/1000000.0);
	double totTime = (t2 - t1);
	cout << totTime << " seconds with Memory Mapping" << endl;
	for(int i=0; i<25; i++)
	{
		close(fd[i]);
	}


	cout << "Opening the files (without memory mapping)" << endl;
	for(int i=0; i<25; i++)
	{
		string name = "file" + to_string(i);
		fd[i] = open(name.c_str(), O_RDWR);
	}

	int fd2;

	// cout << "Before-----------------------------------------\n";
	// system(str.c_str());
	sync();
	fd2 = open("/proc/sys/vm/drop_caches", O_WRONLY);
	write(fd2, data, sizeof(char));
	close(fd2);
	system("cat /proc/sys/vm/drop_caches");
	// cout << "After-----------------------------------------\n";
	// system(str.c_str());

	gettimeofday(&tim, NULL);
	t1 = tim.tv_sec + (tim.tv_usec/1000000.0);

	char buf[512];
	for(int i=0; i<25; i++)
	{
		int numRead = 0;
		while(numRead < 10000000)
		{
			read(fd[i], buf, 512);
			numRead += 512;
		}
	}

	gettimeofday(&tim, NULL);
	t2 = tim.tv_sec + (tim.tv_usec/1000000.0);
	totTime = (t2 - t1);
	cout << totTime << " seconds without Memory Mapping" << endl;

	for(int i=0; i<25; i++)
	{
		close(fd[i]);
	}
	cout << "Exiting" << endl;








	for(int i=0; i<25; i++)
	{
		string name = "file" + to_string(i);
		fd[i] = open(name.c_str(), O_RDWR);
	}

	system(cmmd.c_str());

	int fd3;
	sync();
	fd3 = open("/proc/sys/vm/drop_caches", O_WRONLY);
	write(fd3, data, sizeof(char));
	close(fd3);

	gettimeofday(&tim, NULL);
	t1 = tim.tv_sec + (tim.tv_usec/1000000.0);

	for(int i=0; i<25; i++)
	{
		char* wrPtr = p[i];
		int num = 0;
		while(num < 10000000)
		{
			*(wrPtr) = '1';
			wrPtr++;
			num += 1;

		}
	}

	gettimeofday(&tim, NULL);
	t2 = tim.tv_sec + (tim.tv_usec/1000000.0);
	totTime = (t2 - t1);
	cout << totTime << " seconds writing with Memory Mapping" << endl;
	for(int i=0; i<25; i++)
	{
		close(fd[i]);
	}


	cout << "Opening the files (without memory mapping)" << endl;
	for(int i=0; i<25; i++)
	{
		string name = "file" + to_string(i);
		fd[i] = open(name.c_str(), O_RDWR);
	}

	int fd4;

	// cout << "Before-----------------------------------------\n";
	// system(str.c_str());
	sync();
	fd4 = open("/proc/sys/vm/drop_caches", O_WRONLY);
	write(fd4, data, sizeof(char));
	close(fd4);
	system("cat /proc/sys/vm/drop_caches");
	// cout << "After-----------------------------------------\n";
	// system(str.c_str());

	gettimeofday(&tim, NULL);
	t1 = tim.tv_sec + (tim.tv_usec/1000000.0);

	for(int i=0; i<25; i++)
	{
		int numRead = 0;
		while(numRead < 10000000)
		{
			write(fd[i], buf, 512);
			numRead += 512;
		}
	}

	gettimeofday(&tim, NULL);
	t2 = tim.tv_sec + (tim.tv_usec/1000000.0);
	totTime = (t2 - t1);
	cout << totTime << " seconds writing without Memory Mapping" << endl;

	for(int i=0; i<25; i++)
	{
		close(fd[i]);
	}
	cout << "Exiting" << endl;



	return 0;
}