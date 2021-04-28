#ifndef LBL_H_INCLUDED
#define LBL_H_INCLUDED

/****** Constantes y tipos de dato ******/
#define BLOCK_SIZE (sizeof(int) * 256) // Se define el tamaño del bloque a 1k.

/* Este tipo de dato, define una estructura de una lista doblemente ligada, la cual contendra la dirreccion del siguiente y anterior elemento de la lista, 
el ID del bloque libre y la dirección del bloque libre. */
typedef struct blockitem {
    struct blockitem *next;
    struct blockitem *prev;
	int numeroBloque;
	int *direccion_bloque;  
} BLOCKITEM;

/* Este tipo de dato define la estructura que contiene la direccion del primer y ultimo elemento de la lista doblemente ligada que es la LBL */
typedef struct {
    struct blockitem *first;
    struct blockitem *last;
} BLOCKHDR;

/* Este tipo de dato define la estructura que contiene la direccion del siguiente bloque libre que no esta en la LBL y el ID de este bloque libre */
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

