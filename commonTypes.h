/* File types masks */
#define TYPE_EMPTY  0x0   //empty inode
#define TYPE_IFREG    0x1   //regular file
#define TYPE_DIR    0x2   //directory

#define BLOCK_SIZE 1024

#define fgets_(str) fgets(str, sizeof str, stdin)
#define my_itoa(x) my_itoa_buf(NULL, 0, x)

typedef struct inode
{
    int size;
    char type;
    char owner;
    //int date;
    int numero_bloque;
    int dummy; // usado para llenar los 64 bytes del inodo
    char permissions[6];
    void *contentTable[11];
}inode_t; //ToDo: Definir contenidos del inode

