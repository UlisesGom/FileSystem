// CLiente //
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


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
	char arr1[50];

	printf("InfoDIrectory\n");
	Write2FileSystem("InfoDirectory");

	fdw = open("Send2Fifo", O_RDONLY);
	read(fdw, arr1, sizeof(arr1));
	close(fdw);
	printf("Aquí debería regresar la información del directorio\n ");
	printf("%s\n", arr1);
}

void CreateDirectory(){
	int fdw;

	printf("CreateDirectory");
	Write2FileSystem("CreateDirectorye");
}

void DeleteDirectory(){
	int fdw;

	printf("DeleteDirectory");
	Write2FileSystem("DeleteDirectory");
}

void ChangeDirectory(){
	int fdw;

	printf("ChangeDirectory");
	Write2FileSystem("ChangeDirectory");
}

void CreateFile(){
	int fdw;

	printf("CreateFile");
	Write2FileSystem("CreateFile");
}

void DeleteFile(){
	int fdw;

	printf("DeleteFile");
	Write2FileSystem("DeleteFile");
}

void EditFile(){
	int fdw;

	printf("EditFile");
	Write2FileSystem("EditFile");
}

void OpenFile(){
	int fdw;

	printf("OpenFile");
	Write2FileSystem("OpenFile");
}


void main(void)
{
	int fdw;
	int opciones;
	char arr2[50] = "prueba";

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


