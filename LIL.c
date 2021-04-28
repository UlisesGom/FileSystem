#include <stdlib.h>
#include <stdio.h>
#include "LIL.h"

/********** Variables Globales ***********/
// Esta variable global statica, nos indica el primer y ultimo elemento de la LIL.
static LISTHDR head;
// Esta variable global nos permite revisar la lista de inodos.
extern inode_t inodeList[16][4];

/*********** Funciones ******************/

// Esta funcion nos permite encolar un inodo libre al final de la lista de inodos
void enqueue(LISTITEM *item){
    LISTITEM *temp;

    temp = head.last;
    head.last = item;
    item->prev = temp;
    item->next = (LISTITEM*)&head;
    temp->next = item;
}

// Esta funcion nos permite quitar el primer elemento de la LIL
LISTITEM* dequeue(){
    LISTITEM * temp;

    temp = head.first;
    if(temp == (LISTITEM*)&head)  // Este bloque es el que busca inodos libres cuando la LIL se vacia
    {
        LlenarLIL(inodeList);
        temp = head.first;
        if(temp == (LISTITEM*)&head){ //aqui verifica que si haya inodos disponibles
            temp = NULL;
            }
	else{
        	head.first = temp->next;
	}
    }
    else{
        head.first = temp->next;
        //head.last->prev = (LISTITEM*)&head;
    }
    return temp;
}

// Esta funcion nos permite llenar la LIL tanto para incinicalizarla como para buscar inodos libre cuando la LIL se vacia.
void LlenarLIL(inode_t inodeList[][4]){

	int cuentaLIL = 0;
	LISTITEM *LIL;

    /* Initialize LIL */
    head.first = (LISTITEM*) &head;
    head.last = (LISTITEM*) &head;

	for(int i = 0; i < 4; i++) { // aqui se cambia de bloque para revisar la lista de inodos
		for(int j = 0; j < 16; j++){ //aqui se revisa inodo por inodo
			if((inodeList[j][i].type == TYPE_EMPTY) && (cuentaLIL < 16)) //aqui se revisa el tipo de inodo, si esta vacio sera de tipo 0.
			{
                if ((i == 0) && ((j == 0) | (j ==1))) // Ignora los primeros dos inodos
                {
                    continue;
                }
                else
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
}

// Esta funcion libera un inodo y lo concatena al inicio de la LIL, aqui se hace el remember inode
void freeinode(int inodo){
    LISTITEM* temp;
    temp = (LISTITEM *) malloc(sizeof(LISTITEM)); 
    temp->numeroInodo = inodo%16;
    temp->numeroBloque = inodo/16;
    int contador =0;
    LISTITEM* temp1;
    temp1 = head.first;
    while(temp1 != (LISTITEM*)&head){ // aqui verificamos cuantos elementos de la LIL tenemos
        contador++;
        temp1 = temp1->next;
    }

    
    if(contador < 16){ // Si la lista no esta llena, entonces solo concatena el inodo liberado como el primer elemento de la LIL.
        temp1 = head.first;
        head.first = temp;
        temp->next = temp1;
        temp->prev = (LISTITEM*)&head;
        temp1->prev = temp;
    }
    else{ // Si la lista esta llena entonces revisa el primer elemento de la lista
        temp1 = head.last;
        if(inodo < (temp1->numeroInodo+ (temp1->numeroBloque*16))){ //Este es el remember inode, si el primer elemento de la LIL es mayor que el inode liberado lo sustituye.
            head.last = temp;
            temp->next = temp1->next;
            temp->prev = temp1->prev;
            temp1->prev->next = temp;
        }
    }

}
