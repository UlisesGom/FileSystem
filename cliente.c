// CLiente //
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "commonTypes.h"


/* Declaración de funciones */
void InfoDirectory(void);  
void CreateDirectory(void); 
void DeleteDirectory(void);
void ChangeDirectory(void);
void CreateFile(void);
void DeleteFile(void);
void EditFile(void);
void OpenFile(void);

enum MenuOptions{
	InfoDir,
	CreateDir,
	DeleteDir,
	ChangeDir,
	CreateF,
	DeleteF,
	EditF,
	OpenF
};

		/*fds= open("Write2Client", O_RDONLY);
		fdw = open("Send2Fifo", O_WRONLY);
		write(fdw, arr2, strlen(arr2)+1);
		close(fdw);*/
void Write2FileSystem(char* Data){
	int fdw;

	fdw = open("Send2Fifo", O_WRONLY);
	write(fdw, Data, strlen(Data)+1);
	close(fdw);
}

void InfoDirectory(){

	int fdw;
	char arr[50];

	printf("InfoDIrectory\n");
	Write2FileSystem("ls");

	fdw = open("Send2Fifo", O_RDONLY);
	read(fdw, arr, sizeof(arr));
	close(fdw);
	printf("Aquí debería regresar la información del directorio\n ");
	printf("%s\n", arr);
}

void CreateDirectory(){
	char arr[20];
	char arr2send[50] = "mkdir";

	printf("Escribe el nombre del nuevo directorio \n");
	fgets_(arr);
	strcat(arr2send, " ");
	strcat(arr2send, arr);

	Write2FileSystem(arr2send);
}

void DeleteDirectory(){

	printf("DeleteDirectory");
	Write2FileSystem("rm");
}

void ChangeDirectory(){

	printf("ChangeDirectory");
	Write2FileSystem("cd");
}

void CreateFile(){

	printf("CreateFile");
	Write2FileSystem("CreateFile");
}

void DeleteFile(){

	printf("DeleteFile");
	Write2FileSystem("DeleteFile");
}

void EditFile(){

	printf("EditFile");
	Write2FileSystem("EditFile");
}

void OpenFile(){

	printf("OpenFile");
	Write2FileSystem("OpenFile");
}


void main(void)
{
	int opciones;
	char arr2[50] = "prueba";
	char c;

	while(1)
	{
		printf("Escoge una opción\n");
		printf("----- Escoge que deseas hacer -----\n");
		printf("-----------Directorios--------------\n");
		printf("0.- Información de los Directorios\n");
		printf("1.- Crear un nuevo directorio\n");
		printf("2.- Borrar un directorio\n");
		printf("3.- Cambiar directorio\n");
		printf("----------Archivos-----------------\n");
		printf("4.- Crear un nuevo archivo\n");
		printf("5.- Borrar un archivo\n");
		printf("6.- Editar un archivo\n");
		printf("7.- Abrir un archivo\n");

		scanf("%d", &opciones);
		while((c = getchar()) != '\n' && c != EOF);

		switch(opciones)
		{
			case InfoDir:
				InfoDirectory();
				break;
			case CreateDir:
				CreateDirectory();
				break;
			case DeleteDir:
				DeleteDirectory();
				break;
			case ChangeDir:
				ChangeDirectory();
				break;
			case CreateF:
				CreateFile();
				break;
			case DeleteF:
				DeleteFile();
				break;
			case EditF:
				EditFile();
				break;
			case OpenF:
				OpenFile();
				break;
			default:
				printf("Escoge una opción valida");
				break;
		}
	}
}


