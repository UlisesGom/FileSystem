#include "LBL.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define Total_Bloques 256 // el primer bloque libre es el noveno hasta el 256 que es el ultimo.

/********** Variables Globales ***********/

static BLOCKHDR head_block;
static LISTABLOQUES *listhead;

/*********** Funciones ******************/
void LBL_Refill(BLOCKITEM *Ultimo);
void Bloques_ligados(LISTABLOQUES *Ultimo);


void enqueue_block(BLOCKITEM *item){
    BLOCKITEM *temp;

    temp = head_block.last;
    head_block.last = item;
    item->prev = temp;
    item->next = (BLOCKITEM*)&head_block;
    temp->next = item;
}

BLOCKITEM* dequeue_block()
{
    BLOCKITEM * temp = head_block.first;
    BLOCKITEM * temp_1 = temp->next;
    
    // Este bloque es el como el remember inode
    if(temp_1 == (BLOCKITEM*)&head_block)
    { 
            LBL_Refill(temp);
            head_block.first = temp->next;
            head_block.last->prev = (BLOCKITEM*)&head_block;
    }
    else
    {
        head_block.first = temp->next;
        head_block.last->prev = (BLOCKITEM*)&head_block;
    }

    return temp;
}

void LlenarLBL(void){

    BLOCKITEM *LBL;

    /* Initialize LBL */
    head_block.first = (BLOCKITEM*) &head_block;
    head_block.last = (BLOCKITEM*) &head_block;

    for(int i = 9; i < 265; i++)
    { 
        LBL = (BLOCKITEM *) malloc(sizeof(BLOCKITEM));
        LBL->numeroBloque = i;
        LBL->direccion_bloque = malloc(sizeof(int) * 256); // Aqui se crean los primeros bloques en la memoria que no estan utilizados por el SO.
        enqueue_block(LBL);
    }

    Bloques_ligados((LISTABLOQUES *)LBL->direccion_bloque);
}

void Bloques_ligados(LISTABLOQUES *Ultimo){// aqui asignamos los bloques que no tienen contenido y no estan en la LBL, por lo que cada bloque esta ligado al siguiente.

    LISTABLOQUES *temp;

    listhead = NULL;

    for(int i = 600; i > 264; i--)
    {
        temp = (LISTABLOQUES *)malloc(sizeof(int) * 256);
        temp->numeroBloque = i;
        temp->next = listhead;
        listhead = temp;
    }

    temp = Ultimo;
    temp->numeroBloque = 264;
    temp->next = listhead;

}

void LBL_Refill(BLOCKITEM *Ultimo)
{
    LISTABLOQUES *temp;
    int cuenta = 0;

    temp = (LISTABLOQUES *)Ultimo->direccion_bloque;

    while((temp->next != NULL) && (cuenta < 255))
    {
       temp = temp->next;
       listhead = temp;
       Ultimo = (BLOCKITEM *) malloc(sizeof(BLOCKITEM));
       Ultimo->numeroBloque = temp->numeroBloque;
       Ultimo->direccion_bloque = (int*) temp->next;
       enqueue_block(Ultimo);
       cuenta++;
    }

}

void freeblock(int numero_bloque, LISTABLOQUES* direccion){

    LISTABLOQUES* temp;
    temp = (LISTABLOQUES *)head_block.last->direccion_bloque;
    
    direccion->numeroBloque = numero_bloque;
    direccion->next = listhead;
    temp->next = direccion;
    listhead = temp;
}
