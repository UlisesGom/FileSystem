
/***********************************************************************************

file system: superbloque, bootblock, lista de inode, bloque de datos.

Bloques de 1K
Nuestro sistema tendra 1 GB de informacion.

1K = Boot Block
1K = LBL
1K = 16 # de inodos libres | Informacion del sistema 
4K = Lista de inodos


***********************************************************************************/


#include <stdio.h>

typedef struct inode
{
	int size;
	char type;
	char owner;
	//int date;
	int dummy; // usado para llenar los 64 bytes del inodo
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
	char boot[1024];
	int LBL[256];
	int LIL[16] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18}; // ToDo: Cambiar por una lista doblemente enlazada 
	inode_t inodeList[16][4] = {{}}; 
	dir_t root[64] = {{2, "."}, {2, ".."}}; 


	printf("****************** MENU *******************\n");
}