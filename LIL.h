
#include "commonTypes.h"

/****** Constantes y tipos de dato ******/

typedef struct listitem {
    struct listitem *next;
    struct listitem *prev;
    int numeroInodo;
	int numeroBloque;
} LISTITEM;

typedef struct {
    struct listitem *first;
    struct listitem *last;
} LISTHDR;

/****** Funciones Globales **************/ 

void enqueue(LISTITEM *item);
LISTITEM* dequeue();
void LlenarLIL(inode_t inodeList[][4]);
void freeinode(int inodo);
