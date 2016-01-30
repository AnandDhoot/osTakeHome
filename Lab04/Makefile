OK_COLOR=\x1b[32;01m
NO_COLOR=\033[0m
CO_STRING=$(OK_COLOR)[Compiling..]$(NO_COLOR)
CL_STRING=$(OK_COLOR)[Cleaning Up..]$(NO_COLOR)
all:
	@/bin/echo -e "$(CO_STRING)"
	@g++ -w -std=c++11 -o server-slow server-slow.c
	@gcc -w  -o get-one-file get-one-file.c
	@gcc -w  -o get-one-file-sig get-one-file-sig.c
clean: 
	@/bin/echo -e "$(CL_STRING)"
	@rm server-slow get-one-file-sig get-one-file
