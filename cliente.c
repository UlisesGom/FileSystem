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

	Write2FileSystem("ls");
	fdw = open("Send2Fifo", O_RDONLY);
	read(fdw, arr, sizeof(arr));
	close(fdw);
	printf("----------Directorios-------------\n");
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

	char arr[20];
	char arr2send[50] = "rm";

	printf("Escribe el nombre del directorio a eliminar \n");
	fgets_(arr);
	strcat(arr2send, " ");
	strcat(arr2send, arr);

	Write2FileSystem(arr2send);
}

void ChangeDirectory(){

	char arr[20];
	char arr2send[50] = "cd";

	printf("Escribe a que directorio quieres ir \n");
	fgets_(arr);
	strcat(arr2send, " ");
	strcat(arr2send, arr);

	Write2FileSystem(arr2send);
}

void CreateFile(){

	char arr[20];
	char arr2send[50] = "cat";

	printf("Escribe el nombre del archivo que quieres crear \n");
	fgets_(arr);
	strcat(arr2send, " ");
	strcat(arr2send, arr);

	Write2FileSystem(arr2send);
}

void DeleteFile(){

	char arr[20];
	char arr2send[50] = "del";

	printf("Escribe el nombre del archivo que quieres eliminar \n");
	fgets_(arr);
	strcat(arr2send, " ");
	strcat(arr2send, arr);

	Write2FileSystem(arr2send);
}

void EditFile(){

	char arr[10], texto[40], read_arr[50];
	int opcion, fdw;
	char arr2send[50] = "edit";

	printf("Escribe el nombre del archivo que quieres editar \n");
	fgets_(arr);
	printf("Escribe el texto que quieres agregar \n");
	fgets_(texto);

	strcat(arr2send, " ");
	strcat(arr2send, arr);
	strcat(arr2send, " ");
	strcat(arr2send, texto);
	strcat(arr2send, "*");
	Write2FileSystem(arr2send);

	fdw = open("Send2Fifo", O_RDONLY);
	read(fdw, read_arr, sizeof(read_arr));
	close(fdw);
	printf("\n\n%s\n", read_arr);
}

/*int read_fifo(){
	char read_arr[50] = "";
	int fdw;

	fdw = open("Send2Fifo", O_RDONLY);
	read(fdw, read_arr, sizeof(read_arr));
	close(fdw);
	printf("%s\n", read_arr);
	if(strcmp(read_arr, "$") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}*/

void OpenFile(){

	char arr[20];
	char arr2send[50] = "open";
	char read_arr[50]="";
	//int x=1;
	int fdw;


	printf("Escribe el nombre del archivo que quieres abrir \n");
	fgets_(arr);
	strcat(arr2send, " ");
	strcat(arr2send, arr);

	Write2FileSystem(arr2send);

	fdw = open("Send2Fifo", O_RDONLY);
	read(fdw, read_arr, sizeof(read_arr));
	close(fdw);
	printf("\n%s\n", read_arr);

	/*do
	{
		x = read_fifo();
	}while(x);*/


}


void main(void)
{
	int opciones;
	char c, wait[5];
	char f_arr[20];
	char f_arr2send[50] = "user";
	printf("Bienvenido, por favor ingresa tu nombre de usuario\n");
	fgets_(f_arr);
	strcat(f_arr2send, " ");
	strcat(f_arr2send, f_arr);
	Write2FileSystem(f_arr2send);


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
		printf("\n\n Presiona Enter para continuar \n\n");
		fgets_(wait);

	}
}


