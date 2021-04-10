/* File types masks */
#define TYPE_EMPTY  0x0   //empty inode
#define TYPE_IFREG    0x1   //regular file
#define TYPE_DIR    0x2   //directory

#define BLOCK_SIZE 1024

typedef struct inode
{
    int size;
    char type;
    char owner;
    //int date;
    int dummy[2]; // usado para llenar los 64 bytes del inodo
    char permissions[6];
    void *contentTable[11];
}inode_t; //ToDo: Definir contenidos del inode
