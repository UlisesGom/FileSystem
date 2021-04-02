/* File types masks */
#define S_IFEMPTY  0x0   //empty inode
#define S_IFREG    0x1   //regular file
#define S_IFDIR    0x2   //directory

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
