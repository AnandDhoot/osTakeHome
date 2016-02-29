OK_COLOR=\x1b[32;01m
NO_COLOR=\033[0m
CO_STRING=$(OK_COLOR)[Compiling..]$(NO_COLOR)
CL_STRING=$(OK_COLOR)[Cleaning Up..]$(NO_COLOR)
all:
	@/bin/echo -e "$(CO_STRING)"
	@gcc -std=c99 -w -pthread -o races-partA races-partA.c 
	@gcc -std=c99 -w -pthread -o races-partB races-partB.c
	@gcc -std=c99 -w -pthread -o races-partC races-partC.c
clean:
	@/bin/echo -e "$(CL_STRING)"
	@rm races-partA races-partB races-partC *~ *.o