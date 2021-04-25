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
#include <sys/types.h>
#include <sys/stat.h>


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
char owners[15] ={};
int User_Hash = -1;

/***************** Funciones ****************************************/
void InfoDirectorio(void);
void CrearDirectorio(char* dirName);
int BorrarDirectorio(char* targetDir);
int CambiarDirectorio(char* targetDir);
int EditarArchivo(char* targetDir, char* content);
int AbrirArchivo(char* targetDir);
int BorrarArchivo(char* fileName);
void ProcesarComando(char* buffer);

int Owner_Hash(void){
    static int Hash_Func = 0;
    static char Tabla_Hash[10][15];
    int Codigo_Hash = 0;

    if(Hash_Func == 0){
        for(int i=0; i<10; i++){
                for(int j=0; j<15;j++){
                        Tabla_Hash[i][j] = 0;
                }
        }
        Hash_Func++;


    }

    if(owners[0] != 0){
        for(int i = 0; i<Hash_Func;i++){
            if((strcmp(Tabla_Hash[i], owners))==0){
                Codigo_Hash = i;

                break;
            }
            else if(((Hash_Func-1)==i) &&(Hash_Func < 11)){

                strcpy(Tabla_Hash[Hash_Func-1],owners);
                Codigo_Hash = Hash_Func-1;
                Hash_Func++;

                break;
            }
            else if((Hash_Func-1)==i){
                Codigo_Hash = -1;
            }
            else{
                //idle condition
            }
        }

    }

    return Codigo_Hash;

}
/* Esta función nos permite convertir enteros a arrays para poder
mandarlos a través de la FIFO */
char *my_itoa_buf(char *buf, size_t len, int num)
{
  static char loc_buf[sizeof(int) * 8]; 

  if (!buf)
  {
    buf = loc_buf;
    len = sizeof(loc_buf);
  }

  if (snprintf(buf, len, "%d", num) == -1)
    return ""; 

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
    char bufferSalida[80] = "";

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
    #ifndef TEST_MODE
    	Write2Cliente(bufferSalida);
    #endif
}

void CrearDirectorio(char* dirName)
{
    /* Get a free inode from the LIL and get free block*/
    LISTITEM* itemLIL = dequeue();
    BLOCKITEM* itemLBL = dequeue_block();
    dir_t *dirBLock = (dir_t*)itemLBL->direccion_bloque;
    inode_t* newInode = &inodeList[itemLIL->numeroInodo][itemLIL->numeroBloque];
    dir_t *currentDirBlock = currentDir->contentTable[0];

    /* Initialize new Inode */
    newInode->contentTable[0] = dirBLock;
    newInode->type = TYPE_DIR;
    newInode->numero_bloque = itemLBL->numeroBloque;
    newInode->owner = User_Hash;

    /* Initialize current and top directory */
    memset(dirBLock, 0, BLOCK_SIZE); // Clean memory space for the directory
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
            inode_t *currInode = &inodeList[tempDir->inode % 16][tempDir->inode / 16];
            if((currInode ->owner == User_Hash) && (currInode->type == TYPE_DIR)){ // verifica que tenga permisos
                currInode->type = TYPE_EMPTY;
                freeinode(tempDir->inode);
                tempDir->inode = 0;
                freeblock(currInode->numero_bloque, (LISTABLOQUES *)currInode->contentTable[0]);
                return 0;
            }
            else
            {
                return 1; // Error code "Sin permisos"
            }
        }
        else
        {
            tempDir++;
        }
    }
    return 2; //Error Code "No existe el directorio"
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

    return 2; //Error Code "No existe el directorio"
}

int EditarArchivo(char* targetDir, char* content)
{
	dir_t *tempDir = currentDir->contentTable[0];
    int bytesNeeded = strlen(content);
    int blocksUsed = 0;

    for (int i = 0; i < 64; i++)
    {
        if(strcmp(targetDir, tempDir->name) == 0)
        {
            inode_t *currInode = &inodeList[tempDir->inode % 16][tempDir->inode / 16];
            if(currInode->owner == User_Hash)
            {
                currInode->size = bytesNeeded;

                printf("Storing %d bytes in %s\n", bytesNeeded, targetDir);

                /* Copia el contenido a los bloques */
                while(bytesNeeded > 0)
                {
                    /* Obtener un nuevo bloque para el contenido */
                    BLOCKITEM* newBlock = dequeue_block();
                    currInode->contentTable[blocksUsed] = newBlock->direccion_bloque;

                    printf("Storing block %d...\n", blocksUsed);

                    /* Usar mas de un bloque cuando el contenido sea mas grande que el bloque mismo */
                    if (bytesNeeded > BLOCK_SIZE)
                    {
                        memcpy(currInode->contentTable[blocksUsed], content, BLOCK_SIZE);
                        content += BLOCK_SIZE; // Apuntar al siguiente fragmento del contenido
                        bytesNeeded -= BLOCK_SIZE;  // Restar el numero de bytes copiados
                        blocksUsed++; // Ir al siguiente bloque
                    }
                    else
                    {
                        memcpy(currInode->contentTable[blocksUsed], content, bytesNeeded);
                        bytesNeeded = 0;
                    }
                }
                return 0;
            }
            else
            {
                return 1; // Error code "Sin permisos"
            }
        }
        else
        {
            tempDir++;
        }
    }
    return 3; //Error Code "No existe el archivo"
}

int AbrirArchivo(char* targetDir)
{
	dir_t *tempDir = currentDir->contentTable[0];
    int blocksOpened = 0;
    char *bufferSalida;

    for (int i = 0; i < 64; i++)
    {
        if(strcmp(targetDir, tempDir->name) == 0)
        {
            inode_t *currInode = &inodeList[tempDir->inode % 16][tempDir->inode / 16];
            if(currInode->owner == User_Hash){
                int remainingBytes = currInode->size;

                /* Imprime el contenido de los bloques */
                while(remainingBytes > 0)
                {
                    /* Imprime todos los bloques usados para guardar el contenido */
                    if(remainingBytes >= BLOCK_SIZE)
                    {
                        #ifdef TEST_MODE
                            printf("%.*s", (int)BLOCK_SIZE, (char *)currInode->contentTable[blocksOpened]);

                        #else
                            Write2Cliente((char *)currInode->contentTable[blocksOpened]);
                        #endif
                        remainingBytes -= BLOCK_SIZE;
                    }
                    else
                    {
                        #ifdef TEST_MODE
                            printf("%.*s", remainingBytes, (char *)currInode->contentTable[blocksOpened]);

                        #else
                            Write2Cliente((char *)currInode->contentTable[blocksOpened]);
                        #endif
                        remainingBytes = 0;
                    }
                    blocksOpened++; // Avanza al siguiente bloque
                }
                return 0;
            }
            else
            {
                return 1; // Error code "Sin permisos"
            }
        }
        else
        {
            tempDir++;
        }
    }
    return 3; //Error Code "No existe el archivo"
}


int CrearArchivo(char* fileName)
{
      /* Get a free inode from the LIL and get free block*/
    LISTITEM* itemLIL = dequeue();
    inode_t* newInode = &inodeList[itemLIL->numeroInodo][itemLIL->numeroBloque];
    dir_t *currentDirBlock = currentDir->contentTable[0];

	/* Initialize new Inode */
    newInode->type = TYPE_FILE;
    newInode->owner = User_Hash;
    newInode->size = 0;
	 /* Search for an available space in the current directory */
    for (int i = 0; i < 64; i++)
    {
        if(currentDirBlock->inode == 0)
        {
            currentDirBlock->inode = itemLIL->numeroInodo + (itemLIL->numeroBloque * 16);
            strcpy(currentDirBlock->name, fileName);
            break;
        }
        else
        {
            currentDirBlock++;
        }
    }
}

int BorrarArchivo(char* fileName)
{
    dir_t *tempDir = currentDir->contentTable[0];

    for (int i = 0; i < 64; i++)
    {
        if(strcmp(fileName, tempDir->name) == 0)
        {
            inode_t *currInode = &inodeList[tempDir->inode % 16][tempDir->inode / 16];
            if((currInode->owner == User_Hash) && (currInode->type == TYPE_FILE)){
                currInode->type = TYPE_EMPTY;
                freeinode(tempDir->inode);
                tempDir->inode = 0;
                freeblock(currInode->numero_bloque, (LISTABLOQUES *)currInode->contentTable[0]);
                return 0;
            }
            else
            {
                return 1; // Error code "Sin permisos"
            }
        }
        else
        {
            tempDir++;
        }
    }
    return 3; //Error Code "No existe el archivo"
}

int CambiarUser(char* Usuario)
{
    strcpy(owners, Usuario);
    User_Hash = Owner_Hash();
}

void ErrorCode(int code)
{
	switch(code)
	{
		case 0:
			Write2Cliente("Comando Exitoso");
			break;
		case 1:
			Write2Cliente("Error: No tienes los permisos adecuados");
			break;
		case 2:
			Write2Cliente("Error: No existe el directorio");
			break;
		case 3:
			Write2Cliente("Error: No existe el archivo");
			break;
		default:
			// do nothing
		break;

	}

}

void ProcesarComando(char* buffer){

    char *comando, *parametro, *texto;
    int ret;

    comando = strtok(buffer, " ");
    parametro = strtok(NULL, " ");
    texto = strtok(NULL, "*");

    if (parametro != NULL)
    {
        int len = strlen(parametro);
        if( parametro[len-1] == '\n') parametro[len-1] = 0;
    }

    ret = 5; /*Valor default para no regresar nada al cliente */

	if(strcmp(comando, "ls") == 0){
		InfoDirectorio();
	}
	else if(strcmp(comando, "mkdir") == 0){
		CrearDirectorio(parametro);
	}
	else if(strcmp(comando, "rm") == 0){
		ret = BorrarDirectorio(parametro);
	}
	else if(strcmp(comando, "cd") == 0){
		ret = CambiarDirectorio(parametro);
	}
	else if(strcmp(comando, "cat") == 0){
		CrearArchivo(parametro);
	}
	else if(strcmp(comando, "del") == 0){
		ret = BorrarArchivo(parametro);
	}
	else if(strcmp(comando, "edit") == 0){
		ret = EditarArchivo(parametro, texto);
	}
	else if(strcmp(comando, "open") == 0){
		ret = AbrirArchivo(parametro);
	}
	else if(strcmp(comando, "user") == 0){
		CambiarUser(parametro);
	}

	#ifndef TEST_MODE
		ErrorCode(ret);
	#endif
}


int main(void)
{

    char boot[1024];
    //int LBL[256];
    dir_t root[64] = {{2, "."}, {2, ".."}};
    char buffer[50] = "", buffer2[15];
    int fd;

    /* Inicializa el directorio root*/
    inodeList[2][0].contentTable[0] = root;
    inodeList[2][0].type = TYPE_DIR;

    LlenarLIL(inodeList);
    LlenarLBL();

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
