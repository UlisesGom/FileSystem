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
#include "LBL.h"


/************** Tipos y Constantes **********************************/ 
//#define TEST_MODE 

typedef struct dir
{
    int inode;
    char name[12];
}dir_t;

/**************** Variables Globales ********************************/

inode_t inodeList[16][4] = {{}};
inode_t* currentDir = &inodeList[2][0]; // Root

/***************** Funciones ****************************************/
void InfoDirectorio(void);
void CrearDirectorio(char* dirName);
int BorrarDirectorio(char* targetDir);
int CambiarDirectorio(char* targetDir);
void EditarArchivo(void);
void AbrirArchivo(void);
void ProcesarComando(char* buffer);

char *my_itoa_buf(char *buf, size_t len, int num)
{
  static char loc_buf[sizeof(int) * 8]; /* not thread safe */

  if (!buf)
  {
    buf = loc_buf;
    len = sizeof(loc_buf);
  }

  if (snprintf(buf, len, "%d", num) == -1)
    return ""; /* or whatever */

  return buf;
}

void Write2Cliente(char* Data){
	int fdw;

	fdw = open("Send2Fifo", O_WRONLY);
	write(fdw, Data, strlen(Data)+1);
	close(fdw);
}

void InfoDirectorio()
{
    dir_t *dirBLock = (dir_t*)currentDir->contentTable[0];
    char bufferSalida[80];

    printf("*************************************************\n");
    for (int i = 0; i < 64; i++)
    {
        if (dirBLock->inode != 0)
        {

        	#ifdef TEST_MODE
        		printf("%d   %s\n", dirBLock->inode, dirBLock->name);
        	#else
            strcat(bufferSalida, my_itoa(dirBLock->inode));
            strcat(bufferSalida, "   ");
            strcat(bufferSalida, dirBLock->name);
            strcat(bufferSalida, "\n");
            #endif
            
        }
    dirBLock++;

    }
    Write2Cliente(bufferSalida);
}

void CrearDirectorio(char* dirName)
{
    /* Get a free inode from the LIL and get free block*/
    LISTITEM* itemLIL = dequeue();
    BLOCKITEM* itemLBL = dequeue_block();
    dir_t *dirBLock = (dir_t)itemLBL.direccion_bloque;
    inode_t* newInode = &inodeList[itemLIL->numeroInodo][itemLIL->numeroBloque];
    dir_t *currentDirBlock = currentDir->contentTable[0];

    /* Initialize new Inode */
    newInode->contentTable[0] = dirBLock;
    newInode->type = TYPE_DIR;
    newInode->numero_bloque = itemLBL->numeroBloque;

    /* Initialize current and top directory */
    strcpy(&dirBLock->name[0], ".");
    dirBLock->inode = itemLIL->numeroInodo + (itemLIL->numeroBloque * 16);
    dirBLock++;
    strcpy(&dirBLock->name[0], "..");
    dirBLock->inode = currentDirBlock->inode;

    /* Search for an available space in the current directory */
    for (int i = 0; i < 64; i++)
    {
        if(currentDirBlock->inode == 0)
        {
            currentDirBlock->inode = itemLIL->numeroInodo + (itemLIL->numeroBloque * 16);
            strcpy(currentDirBlock->name, dirName);
            break;
        }
        else
        {
            currentDirBlock++;
        }
    }
}

int BorrarDirectorio(char* targetDir)
{
	dir_t *tempDir = currentDir->contentTable[0];

    for (int i = 0; i < 64; i++)
    {
        if(strcmp(targetDir, tempDir->name) == 0)
        {
            inodeList[tempDir->inode % 16][tempDir->inode / 16].type = TYPE_EMPTY;
            tempDir->inode = 0;
            freeblock(inodeList[tempDir->inode % 16][tempDir->inode / 16].numero_bloque, tempDir);
            return 0;
        }
        else
        {
            tempDir++;
        }
    }

    return -1;
}

int CambiarDirectorio(char* targetDir)
{
    dir_t *tempDir = currentDir->contentTable[0];

    for (int i = 0; i < 64; i++)
    {
        if(strcmp(targetDir, tempDir->name) == 0)
        {
            currentDir = &inodeList[tempDir->inode % 16][tempDir->inode / 16];
            return 0;
        }
        else
        {
            tempDir++;
        }
    }

    return -1;
}

void EditarArchivo(){
	printf("EditarArchivo");
}

void AbrirArchivo(){
	printf("AbrirArchivo");
}


void CrearArchivo(char* fileName)
{
    /* Get a free inode from de LIL */
}

void BorrarArchivo(char* fileName)
{
    /* Find the file to be deleted */
    int inodeId;// = BuscarNumeroDeInode(fileName, currentDir);

    if (inodeId != -1)
    {
        inode_t* fileInode = &inodeList[inodeId / 16][inodeId % 16];
        fileInode->type = TYPE_EMPTY;
    }
}

void ProcesarComando(char* buffer){

    char *comando, *parametro;

    comando = strtok(buffer, " ");
    parametro = strtok(NULL, " ");

	if(strcmp(comando, "ls") == 0){
		InfoDirectorio();
	}
	else if(strcmp(comando, "mkdir") == 0){
		CrearDirectorio(parametro);
	}
	else if(strcmp(comando, "rm") == 0){
		BorrarDirectorio(parametro);
	}
	else if(strcmp(comando, "cd") == 0){
		CambiarDirectorio(parametro);
	}
	else if(strcmp(comando, "CreateFile") == 0){
		//CrearArchivo();
		printf("5");
	}
	else if(strcmp(comando, "DeleteFile") == 0){
		//BorrarArchivo();
		printf("6");
	}
	else if(strcmp(comando, "EditFile") == 0){
		EditarArchivo();
		printf("7");
	}
	else if(strcmp(comando, "OpenFile") == 0){
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

    /* Inicializa el directorio root*/
    inodeList[2][0].contentTable[0] = root;
    inodeList[2][0].type = TYPE_DIR;

    LlenarLIL(inodeList);

    #ifdef TEST_MODE
        while(1)
        {
            fgets_(buffer); 
            int len = strlen(buffer);
            if( buffer[len-1] == '\n') buffer[len-1] = 0;
            ProcesarComando(buffer);
        }
    #endif

    /* Inicializa el FIFO */
    mkfifo("Send2Fifo", 0666);

	while(1)
	{
		fd = open("Send2Fifo", O_RDONLY);
		if(fd == -1){
			printf("no se pudo abrir el archivo");
		}
		read(fd,buffer, 50);
		close(fd);
		ProcesarComando(buffer);
	}

    /*
    int fd = open("pruebax", O_WRONLY || O_CREAT);
    write(fd,boot, 1024);
    write(fd, LBL, 1024);
    write(fd,LIL, 1024);
    write(fd, Lista_inode, 1024*4);
    write(fd, raiz, 1024);
    */

    return 0;
}
