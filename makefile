compile: main.o shell.o
	@gcc -Wall -o shell shell.o main.o
main.o: main.c shell.h
	@gcc -c main.c
shell.o: shell.c
	@gcc -c shell.c 
clean:
	@rm -f *.o shell
