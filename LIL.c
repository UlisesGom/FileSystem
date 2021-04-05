#include <stdlib.h>
#include <stdio.h>
#include "LIL.h"

/********** Variables Globales ***********/

static LISTHDR head;

/*********** Funciones ******************/

void enqueue(LISTITEM *item){
    LISTITEM *temp;

    temp = head.last;
    head.last = item;
    item->prev = temp;
    item->next = (LISTITEM*)&head;
    temp->next = item;
}

LISTITEM* dequeue(){
    LISTITEM * temp;

    temp = head.first;
    if(temp == (LISTITEM*)&head){
        temp = NULL;
    }
    else{
        head.first = temp->next;
        head.last->prev = (LISTITEM*)&head;
    }
    return temp;
}

void LlenarLIL(inode_t inodeList[][4]){

	int cuentaLIL = 0;
	LISTITEM *LIL;

    /* Initialize LIL */ 
    head.first = (LISTITEM*) &head;
    head.last = (LISTITEM*) &head;

	for(int i = 0; i < 4; i++) { // aqui se cambia de bloque para revisar la lista de inodos
		for(int j = 0; j < 16; j++){ //aqui se revisa inodo por inodo
			if((inodeList[j][i].type == S_IFEMPTY) && (cuentaLIL <= 16)) //aqui se revisa el tipo de inodo, si esta vacio sera de tipo 0.
			{
				cuentaLIL++; // aqui se lleva la cuenta de cuantos inodos se han registrado en la LIL
				LIL = (LISTITEM *) malloc(sizeof(LISTITEM)); //aqui asinara la direccion a LIL
				LIL -> numeroInodo = j;
				LIL -> numeroBloque = i;
				enqueue(LIL); //Crea el los datos de la lista doblemente enlazada
			}
		}
	}
}