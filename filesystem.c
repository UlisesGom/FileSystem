
/***********************************************************************************

file system: superbloque, bootblock, lista de inode, bloque de datos.

Bloques de 1K
Nuestro sistema tendra 1 GB de informacion.

1K = Boot Block
1K = LBL
1K = 16 # de inodos libres | Informacion del sistema
4K = Lista de inodos


***********************************************************************************/

#include <stdlib.h>
#include <stdio.h>

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

LISTHDR head;

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

typedef struct inode
{
	int size;
	char type;
	char owner;
	//int date;
	int dummy[2]; // usado para llenar los 64 bytes del inodo
	char permissions[6];
	int contentTable[11];
}inode_t; //ToDo: Definir contenidos del inode

typedef struct dir
{
	int inode;
	char name[12];
}dir_t;



int main(void)
{
	LISTITEM *LIL;

    head.first = (LISTITEM*) &head;
    head.last = (LISTITEM*) &head;
	char boot[1024];
	int LBL[256];
	//int LIL[16] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18}; // ToDo: Cambiar por una lista doblemente enlazada

	inode_t inodeList[16][4] = {{}};
	dir_t root[64] = {{2, "."}, {2, ".."}};


	int cuentaLIL = 0;
	for(int i = 0; i<4; i++) { // aqui se cambia de bloque para revisar la lista de inodos
		for(int j = 0; j<16; j++){ //aqui se revisa inodo por inodo
			if((inodeList[j][i].type == 0) && (cuentaLIL <= 16)) //aqui se revisa el tipo de inodo, si esta vacio sera de tipo 0.
			{
				cuentaLIL++; // aqui se lleva la cuenta de cuantos inodos se han registrado en la LIL
				LIL = (LISTITEM *) malloc(sizeof(LISTITEM)); //aqui asinara la direccion a LIL
				LIL -> numeroInodo = j;
				LIL -> numeroBloque = i;
				enqueue(LIL); //Crea el los datos de la lista doblemente enlazada
			}
		}
	}
	/*
	int fd = open("pruebax", O_WRONLY || O_CREAT);
    write(fd,boot, 1024);
    write(fd, LBL, 1024);
    write(fd,LIL, 1024);
    write(fd, Lista_inode, 1024*4);
    write(fd, raiz, 1024);
	*/


	printf("****************** MENU *******************\n");

	return 0;
}
