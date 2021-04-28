
#include "commonTypes.h"

/****** Constantes y tipos de dato ******/

// Este tipo de dato define una structura de una lista doblemente ligada, la cual contrendra el siguiente y anterior elemento de la lista, asi como la info del ID del inodo.
typedef struct listitem {
    struct listitem *next;
    struct listitem *prev;
    int numeroInodo;
    int numeroBloque;
} LISTITEM;

// Este tipo de dato define a una estructura, la cual contiene el primer y ultimo elemento de la LIL.
typedef struct {
    struct listitem *first;
    struct listitem *last;
} LISTHDR;

/****** Funciones Globales **************/ 

void enqueue(LISTITEM *item);
LISTITEM* dequeue();
void LlenarLIL(inode_t inodeList[][4]);
void freeinode(int inodo);
