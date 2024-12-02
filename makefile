compile: shell.o
	@gcc -Wall -o shell shell.o

shell.o: shell
	@gcc -c shell

clean:
	@rm -f *.o shell
