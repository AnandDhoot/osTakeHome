OK_COLOR=\x1b[32;01m
NO_COLOR=\033[0m
CO_STRING=$(OK_COLOR)[Compiling..]$(NO_COLOR)
CL_STRING=$(OK_COLOR)[Cleaning Up..]$(NO_COLOR)
all:
	@/bin/echo -e "$(CO_STRING)"
	@g++ -std=c++11 -w -pthread server-mt.c
clean:
	@/bin/echo -e "$(CL_STRING)"
	@rm a.out *~ *.o