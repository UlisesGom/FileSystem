all: filesystem.c LIL.c
	gcc -o filesystem filesystem.c LIL.c -I.