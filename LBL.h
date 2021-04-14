#ifndef LBL_H_INCLUDED
#define LBL_H_INCLUDED

/****** Constantes y tipos de dato ******/

typedef struct blockitem {
    struct blockitem *next;
    struct blockitem *prev;
	int numeroBloque;
	int *direccion_bloque;
} BLOCKITEM;

typedef struct {
    struct blockitem *first;
    struct blockitem *last;
} BLOCKHDR;

typedef struct listabloques{
    struct listabloques *next;
    int numeroBloque;
} LISTABLOQUES;

/****** Funciones Globales **************/

void enqueue_block(BLOCKITEM *);
BLOCKITEM* dequeue_block();
void LlenarLBL(void);
void freeblock(int, LISTABLOQUES*);

#endif // LBL_H_INCLUDED

