#ifndef FS____H

#define FS____H

#define MFS_DIRECTORY    (0)
#define MFS_REGULAR_FILE (1)

#define VALID_PINUM -1

#define INODE_NUMBER 4096
#define BLOCK_SIZE  4096
#define FILE_MAX_BLOCK_NUM 10


typedef struct __MFS_Stat_t {
    int type;   // MFS_DIRECTORY or MFS_REGULAR
    int size;   // bytes
    int blocks; // number of blocks allocated to file
    // note: no permissions, access times, etc.
} MFS_Stat_t;

typedef struct __MFS_DirEnt_t {
    int  inum;      // inode number of entry (-1 means entry not used)
    char name[252]; // up to 252 bytes of name in directory (including \0)
} MFS_DirEnt_t;


int toggle_bit(int block,unsigned char bitmap[]);

int block_status(int block, unsigned char bitmap[]);

int fs_init(char *filename);
int fs_Lookup(int pinum, char *name);
int fs_Stat(int inum, MFS_Stat_t *m);
int fs_Write(int inum, char *buffer, int block);
int fs_Read(int inum, char *buffer, int block);
int fs_create(int pinum, int type, char *name);
int fs_Unlink(int pinum, char *name);
int fs_close();
#endif

