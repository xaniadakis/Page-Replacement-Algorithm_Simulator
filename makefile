CC=gcc
DEPS=functions.h

all:	project

project:	main.c	functions.c
	gcc	main.c	functions.c	-o project

clean:
		$(RM)	project
