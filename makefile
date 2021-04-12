all: filesystem.c LIL.c cliente.c
	gcc -o filesystem filesystem.c LIL.c LBL.c -I.
	gcc -o cliente cliente.c -I.
