#include "LBL.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define Total_Bloques 256 // el primer bloque libre es el noveno hasta el 256 que es el ultimo.

/********** Variables Globales ***********/

// Esta variable head_block indica el primer elemento de la lista y el ultimo elemento de la LBL
static BLOCKHDR head_block;
// Esta variable listhead indica solo el primer elemento de la lista de bloques libres que no estan en la LBL
static LISTABLOQUES *listhead;

/*********** Funciones ******************/
void LBL_Refill(BLOCKITEM *Ultimo);
void Bloques_ligados(LISTABLOQUES *Ultimo);

//Esta funcion encola los elementos en la LBL
void enqueue_block(BLOCKITEM *item){
    BLOCKITEM *temp;

    temp = head_block.last;
    head_block.last = item;
    item->prev = temp;
    item->next = (BLOCKITEM*)&head_block;
    temp->next = item;
}

// Esta funcion quita el primer elemento de la LBL y regresa su dirección, para utilizar el bloque libre.
BLOCKITEM* dequeue_block()
{
    BLOCKITEM * temp = head_block.first;
    BLOCKITEM * temp_1 = temp->next;

    // Este bloque es el como el remember inode
    if(temp_1 == (BLOCKITEM*)&head_block)
    {
            LBL_Refill(temp);
            head_block.first = temp->next;
            //head_block.last->prev = (BLOCKITEM*)&head_block;
    }
    else
    {
        head_block.first = temp->next;
        //head_block.last->prev = (BLOCKITEM*)&head_block;
    }

    return temp;
}

// Esta funcion se inicializa en el main y crea la LBL con sus 256 bloques libres solo la primera vez
void LlenarLBL(void){

    BLOCKITEM *LBL;

    /* Initialize LBL */
    head_block.first = (BLOCKITEM*) &head_block;
    head_block.last = (BLOCKITEM*) &head_block;

    for(int i = 9; i < 265; i++) // se crea el primer bloque libre con ID 9 ya que los otros 8 ya estan siendo utilizados
    {
        LBL = (BLOCKITEM *) malloc(sizeof(BLOCKITEM));
        LBL->numeroBloque = i;
        LBL->direccion_bloque = malloc(BLOCK_SIZE); // Aqui se crean los primeros bloques en la memoria que no estan utilizados por el SO.
        enqueue_block(LBL);
    }

    Bloques_ligados((LISTABLOQUES *)LBL->direccion_bloque); // Una vez creada la LBL y los bloques libres que apunta, se solicita crear el resto de bloques libres.
}

void Bloques_ligados(LISTABLOQUES *Ultimo){// aqui creamos los bloques que no tienen contenido y no estan en la LBL, cada bloque esta ligado al siguiente.

    LISTABLOQUES *temp;

    listhead = NULL;

    for(int i = 600; i > 264; i--) // aqui se crean el resto de bloques y se ligan entre si, en total tenemos 600 bloques de un 1k.
    {
        temp = (LISTABLOQUES *)malloc(BLOCK_SIZE);
        temp->numeroBloque = i;
        temp->next = listhead;
        listhead = temp;
    }

    temp = Ultimo;
    temp->numeroBloque = 264;
    temp->next = listhead;
    listhead = temp;

}

/* Esta funcion busca bloques libres, utilizando la direccion que tiene el ultimo bloque de la LBL, por lo que cada bloque libre que no esta en la LBL guarda
 la direccion del siguiente. */
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
       Ultimo->direccion_bloque = (int*) temp;
       enqueue_block(Ultimo);
       cuenta++;
    }

}

/* Esta funcion concatena el bloque liberado como el primer bloque a utilizar cuando se terminen los bloques de la LBL, para su reciclado*/
void freeblock(int numero_bloque, LISTABLOQUES* direccion){

    LISTABLOQUES* temp;
    temp = (LISTABLOQUES *)head_block.last->direccion_bloque;

    direccion->numeroBloque = numero_bloque;
    direccion->next = listhead;
    temp->next = direccion;
}
