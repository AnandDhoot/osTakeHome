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

using namespace std;

int main()
{
	int fd = open("file",O_RDONLY);
	int pid = getpid();
	string cmmd = "pmap -x " + to_string(pid) + " | tail -n 1";

	system(cmmd.c_str());

	cout << "Mapping the file" << endl;
	char* p = (char*) mmap (0, 10000000, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED)
	        perror ("mmap");

	system(cmmd.c_str());

	char a = *p;
	cout << "First character read is = " << a << endl;

	system(cmmd.c_str());

	p += 10000;
	a = *p;
	cout << "First character read is = " << a << endl;
	
	system(cmmd.c_str());
}