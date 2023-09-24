all: copy
	gcc -g -Wall -Wextra -o copy copy.o
copy: copy.c
	gcc -g -Wall -Wextra -c copy.c -o copy.o
clean:
	rm copy copy.o
submission:
	zip prog2.zip Makefile copy.c
