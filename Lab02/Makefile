OK_COLOR=\x1b[32;01m
NO_COLOR=\033[0m
CO_STRING=$(OK_COLOR)[Compiling..]$(NO_COLOR)
CL_STRING=$(OK_COLOR)[Cleaning Up..]$(NO_COLOR)
all:
	@/bin/echo -e "$(CO_STRING)"
	@g++ -w -std=c++11 -o server-mp server-mp.c
	@gcc -w -g -pthread -o multi-client multi-client.c
clean:
	@/bin/echo -e "$(CL_STRING)"
	@rm server client
