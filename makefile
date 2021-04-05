all: filesystem.c LIL.c cliente.c
	gcc -o filesystem filesystem.c LIL.c -I.
	gcc -o cliente cliente.c -I.
