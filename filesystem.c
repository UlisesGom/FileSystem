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
#include <string.h>
#include "LIL.h"
#include <unistd.h>
#include <fcntl.h>



typedef struct dir
{
    int inode;
    char name[12];
}dir_t;


int BuscarNumeroDeInode(char* fileName, dir_t *currentDir);
void InfoDirectorio(void);  
void CrearDirectorio(void); 
void BorrarDirectorio(void);
void CambiarDirectorio(void);
void EditarArchivo(void);
void AbrirArchivo(void); 
void ReadData(char* buffer);

inode_t inodeList[16][4] = {{}};

void InfoDirectorio(){
	printf("Info");
}

void CrearDirectorio(){
	printf("CrearDirectorio");
}

void BorrarDirectorio(){
	printf("BorrarDirectorio");
}

void CambiarDirectorio(){
	printf("CambiarDirectorio");
}

void EditarArchivo(){
	printf("EditarArchivo");
}

void AbrirArchivo(){
	printf("AbrirArchivo");
}


void CrearArchivo(char* fileName, char* owner, dir_t *currentDir)
{
    /* Get a free inode from de LIL */
    LISTITEM* itemLIL; //= dequeue();
    inode_t* fileInode = &inodeList[itemLIL->numeroInodo][itemLIL->numeroBloque];

    /* Fill target directory with Inode Data */
    currentDir->inode = (itemLIL->numeroBloque * 16) + itemLIL->numeroInodo;
    strcpy(currentDir->name, fileName);

    /* Initialize Inode Data */
    fileInode->type = S_IFREG;
}

void BorrarArchivo(char* fileName, dir_t *currentDir)
{
    /* Find the file to be deleted */
    int inodeId = BuscarNumeroDeInode(fileName, currentDir);

    if (inodeId != -1)
    {
        inode_t* fileInode = &inodeList[inodeId / 16][inodeId % 16];
        fileInode->type = S_IFEMPTY;
    }
}

int BuscarNumeroDeInode(char* fileName, dir_t *currentDir)
{
    while(currentDir->name[0] != '\0')
    {
        if(strcmp(fileName, currentDir->name))
        {
            return currentDir->inode;
        }
        else
        {
            currentDir++;
        }
    }

    return -1;
}

void ReadData(char* buffer){

	if(strcmp(buffer, "InfoDirectory") == 0){
		InfoDirectorio();
		printf("1");
	}
	else if(strcmp(buffer, "CreateDirectory") == 0){
		CrearDirectorio();
		printf("2");
	}
	else if(strcmp(buffer, "DeleteDirectory") == 0){
		BorrarDirectorio();
		printf("3");
	}
	else if(strcmp(buffer, "ChangeDirectory") == 0){
		CambiarDirectorio();
		printf("4");
	}
	else if(strcmp(buffer, "CreateFile") == 0){
		//CrearArchivo();
		printf("5");
	}
	else if(strcmp(buffer, "DeleteFile") == 0){
		//BorrarArchivo();
		printf("6");
	}	
	else if(strcmp(buffer, "EditFile") == 0){
		EditarArchivo();
		printf("7");
	}
	else if(strcmp(buffer, "OpenFile") == 0){
		AbrirArchivo();
		printf("8");
	}	
}



int main(void)
{

    char boot[1024];
    int LBL[256];    
    dir_t root[64] = {{2, "."}, {2, ".."}};
    char buffer[50] = "";
    int fd;

    mkfifo("Send2Fifo", 0666);


	
	while(1)
	{
	fd = open("Send2Fifo", O_RDONLY);
	if(fd == -1){
		printf("no se pudo abrir el archivo");
	}
	read(fd,buffer, 50);
	close(fd); 
	ReadData(buffer);
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