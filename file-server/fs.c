/* fs.c
Some useful functions for OSP Practical Case Study E
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "fs.h"

//   file max size: 4096* 10 = 40KB
#define FILE_MAX_SIZE (BLOCK_SIZE*FILE_MAX_BLOCK_NUM)

// 4096 * 10 * 4096 = 160MB
#define TOTAL_DATA_SIZE (INODE_NUMBER*FILE_MAX_SIZE)

/* The inode struct */
struct inode
{
    short type;
    char name[256];
    short pinum;
    short size;
    short blockcount;
    short block[FILE_MAX_BLOCK_NUM];
};

// 4096 * 10 * 4096 = 160MB
#define INODE_SIZE sizeof(struct inode)
#define TOTAL_INODE_SIZE INODE_NUMBER*INODE_SIZE
#define TOTAL_DISK_SIZE (TOTAL_DATA_SIZE+TOTAL_INODE_SIZE)

// 4096/8 = 512
#define INODE_BITMAP_NUMBER INODE_NUMBER/8
unsigned char inodebitmap[INODE_BITMAP_NUMBER];

// 4096 * 10 /8 = 5120
#define BLOCK_BITMAP_NUMBER ((BLOCK_SIZE*FILE_MAX_BLOCK_NUM)/8)
unsigned char blockbitmap[BLOCK_BITMAP_NUMBER];

FILE *diskfd;

int toggle_bit(int block, unsigned char bitmap[])
{
    int elem = block / 8;
    int pos = block % 8;
    int mask = 1 << pos;
    bitmap[elem] ^= mask;
    return bitmap[elem] & mask;
}

int block_status(int block, unsigned char bitmap[])
{
    int elem = block / 8;
    int pos = block % 8;
    int mask = 1 << pos;
    return bitmap[elem] & mask;
}

int fs_init(char *filename )
{
    memset( inodebitmap,0,sizeof (inodebitmap));
    memset( blockbitmap,0,sizeof (blockbitmap));

    if( ( access(filename,F_OK)) == -1)
    {
        printf("%s does not exist. create it and properly initialize it to include an empty root directory\n", filename);

        diskfd = fopen( filename,"wb+" );
        fseek(diskfd, TOTAL_DISK_SIZE -1,SEEK_SET);
        fputc(0, diskfd);
        fflush(diskfd);

        struct inode rootinode;
        memset( &rootinode,0,sizeof (struct inode));
        rootinode.type = MFS_DIRECTORY;
        strcpy(rootinode.name,"/");
        rootinode.pinum = 0;
        fseek(diskfd,0,SEEK_SET);
        fwrite(&rootinode, sizeof (struct inode), 1, diskfd);
        fflush(diskfd);
        toggle_bit(0,inodebitmap);

        struct inode tempinode;
        memset( &tempinode,0,sizeof (struct inode));
        tempinode.pinum = -1;
        tempinode.size =0;
        tempinode.blockcount=0;
        for( int i = 0; i < FILE_MAX_BLOCK_NUM ; i ++)
        {
            tempinode.block[i]= -1;
        }
        for( int i = 1; i < INODE_NUMBER ; i ++)
        {
            fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
            fwrite(&tempinode, sizeof (struct inode), 1, diskfd);
            fflush(diskfd);
        }
    }
    else {
        printf("%s exist. use it\n", filename);
        diskfd = fopen( filename,"rb" );

        fseek(diskfd,0,SEEK_END);
        long int file_size = ftell(diskfd);

        if( file_size != TOTAL_DISK_SIZE )
        {
            fprintf(stderr,"open %s valid, maybe the block is damaged!!!\n",filename);
            return -1;
        }

        struct inode tempinode;
        memset( &tempinode,0,sizeof (struct inode));
        for( int i = 0; i < 3 ; i ++)
        {
            fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
            fread(&tempinode, sizeof (struct inode), 1, diskfd);
            if(tempinode.pinum >= 0 || i == 0 )
            {
                toggle_bit(i,inodebitmap);
                if(tempinode.type  == MFS_REGULAR_FILE )
                {
                    for( int j = 0; j < FILE_MAX_BLOCK_NUM ; j ++)
                    {
                        printf("ewrsdf %d\n", tempinode.block[j]);
                        if( tempinode.block[j] != -1 )
                            toggle_bit(j,blockbitmap);
                    }
                }

            }
        }
    }
    return 0;
}

int fs_Lookup( int pinum, char *name )
{
    if( pinum <0 || pinum > INODE_NUMBER )
        return  -1;

    if( block_status(pinum, inodebitmap) != 0 )
    {
        struct inode pinode;
        memset( &pinode,0,sizeof (struct inode));
        fseek(diskfd, pinum*sizeof(struct inode),SEEK_SET);
        fread(&pinode, sizeof (struct inode), 1, diskfd);
        if(  pinode.pinum >= 0)
        {
            if(pinode.type  == MFS_DIRECTORY )
            {
                if( strcmp( name, ".") == 0  || strcmp( name, "..") == 0 )
                    return pinum;

                struct inode childinode;
                memset( &childinode,0,sizeof (struct inode));
                for(int i=0;i<INODE_NUMBER;i++)
                {
                    if( block_status(i,inodebitmap) != 0 )
                    {
                        fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
                        fread(&childinode, sizeof (struct inode), 1, diskfd);
                        printf("%d pinum %d %d %s %s\n",  i, childinode.pinum , pinum ,name,childinode.name );
                        if( childinode.pinum == pinum && strcmp(name,childinode.name )==0)
                        {
                            printf("find child \n");
                            return i;
                        }
                    }
                }
            }
        }
    }

    return  -1;
}

int fs_Stat(int inum, MFS_Stat_t *m)
{
    if( inum <0 || inum >= INODE_NUMBER )
        return  -1;

    if( block_status(inum, inodebitmap) != 0 )
    {
        struct inode currentinode;
        memset( &currentinode,0,sizeof (struct inode));
        fseek(diskfd,inum*sizeof(struct inode),SEEK_SET);
        fread(&currentinode, sizeof (struct inode), 1, diskfd);

        m->type = currentinode.type;

        if( currentinode.type ==  MFS_DIRECTORY )
        {
            int temp = 0;
            for(int i=1;i<INODE_NUMBER;i++)
            {
                struct inode childinode;
                memset( &childinode,0,sizeof (struct inode));

                if( block_status(i,inodebitmap) != 0 )
                {
                    fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
                    fread(&childinode, sizeof (struct inode), 1, diskfd);
                    if(  childinode.pinum == inum )
                        temp++;
                }
            }

            temp = temp + 2;
            m->blocks= 1;
            m->size = temp * sizeof(MFS_DirEnt_t);
        }
        else {
            m->blocks= currentinode.blockcount;
            m->size = currentinode.size;
        }
        printf(" size %d type %d blocks %d \n", m->size,m->type,m->blocks);
        return 0;
    }

    return -1;
}
int fs_Write(int inum, char *buffer, int block)
{
    if( inum < 0 || inum >= INODE_NUMBER || block < 0 || block >= 10 )
        return  -1;

    if( block_status(inum, inodebitmap) != 0 )
    {
        struct inode currentinode;
        memset( &currentinode,0,sizeof (struct inode));
        fseek(diskfd,inum*sizeof(struct inode),SEEK_SET);
        fread(&currentinode, sizeof (struct inode), 1, diskfd);

        if(  currentinode.pinum >= 0)
        {
            if(currentinode.type == MFS_REGULAR_FILE )
            {
                if(currentinode.blockcount + 1 < FILE_MAX_BLOCK_NUM )
                {
                    for(int i=0;i<FILE_MAX_SIZE;i++)
                    {
                        if( block_status(i,blockbitmap) == 0 )
                        {
                            fseek(diskfd, TOTAL_INODE_SIZE+ i*BLOCK_SIZE,SEEK_SET);
                            fwrite(buffer, BLOCK_SIZE, 1, diskfd);
                            fflush(diskfd);
                            toggle_bit(i,blockbitmap);
                            currentinode.size=  currentinode.size + BLOCK_SIZE;

                            if( currentinode.block[block] == -1 )
                            {
                                currentinode.blockcount++;
                            }

                            printf("inum %d, blockindex: %d write block  %d\n", inum,block, i);

                            currentinode.block[block] = i;

                            fseek(diskfd, inum*sizeof(struct inode),SEEK_SET);
                            fwrite(&currentinode, sizeof (struct inode), 1, diskfd);
                            fflush(diskfd);

                            return 0;
                        }
                    }
                }

            }


        }
    }
    else {
        printf("xxx");
        return  -1;
    }
    return  -1;
}

int fs_Read(int inum, char *buffer, int block)
{
    printf("inum %d  block %d  stat %d  \n", inum,block ,block_status(inum, inodebitmap));
    if( inum <0 || inum >= INODE_NUMBER || block < 0 || block >= 10 )
    {
        printf("xxxx");
        return  -1;
    }

    if( block_status(inum, inodebitmap) != 0 )
    {
        struct inode currentinode;
        memset( &currentinode,0,sizeof (struct inode));
        fseek(diskfd,inum*sizeof(struct inode),SEEK_SET);
        fread(&currentinode, sizeof (struct inode), 1, diskfd);

        if(  currentinode.pinum >= 0)
        {
            if(currentinode.type == MFS_REGULAR_FILE )
            {
                if( currentinode.block[block] != -1 )
                {
                    fseek(diskfd, TOTAL_INODE_SIZE+ currentinode.block[block]*BLOCK_SIZE,SEEK_SET);
                    fread(buffer, BLOCK_SIZE, 1, diskfd);
                    return 0;
                }
            }
            else {
                if( block != 0 )
                    return  -1;

                int find = 0;
                for(int i=1;i<INODE_NUMBER;i++)
                {
                    struct inode newinode;
                    memset( &newinode,0,sizeof (struct inode));

                    if( block_status(i,inodebitmap) != 0 )
                    {
                        fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
                        fread(&newinode, sizeof (struct inode), 1, diskfd);
                        if(  newinode.pinum == inum )
                        {
                            MFS_DirEnt_t mfsdirent;
                            mfsdirent.inum=i;
                            memcpy( mfsdirent.name, newinode.name, 252);

                            memcpy( buffer + find*sizeof (mfsdirent), &mfsdirent, sizeof (mfsdirent));
                            find++;
                        }
                    }
                }

                MFS_DirEnt_t mfsdirent;
                mfsdirent.inum=inum;
                strcpy( mfsdirent.name, ".");

                memcpy( buffer + find*sizeof (mfsdirent), &mfsdirent, sizeof (mfsdirent));
                find++;

                mfsdirent.inum=inum;
                strcpy( mfsdirent.name, "..");

                memcpy( buffer + find*sizeof (mfsdirent), &mfsdirent, sizeof (mfsdirent));
                find++;

                printf("findfindfindfind %d",find);
                return 0;
            }
        }
    }

    return  -1;

}
int fs_create(int pinum, int type, char *name)
{
    if( pinum < 0 || pinum >= INODE_NUMBER )
        return  -1;

    if( block_status(pinum, inodebitmap) != 0 )
    {
        struct inode pinode;
        memset( &pinode,0,sizeof (struct inode));
        fseek(diskfd, pinum*sizeof(struct inode),SEEK_SET);
        fread(&pinode, sizeof (struct inode), 1, diskfd);
        if(  pinode.pinum >= 0)
        {
            if(pinode.type == MFS_DIRECTORY )
            {
                struct inode childinode;
                memset( &childinode,0,sizeof (struct inode));
                childinode.type = type;
                strcpy(childinode.name,name);
                childinode.pinum = pinum;
                childinode.size =0;
                childinode.blockcount=0;
                for( int i = 0; i < FILE_MAX_BLOCK_NUM ; i ++)
                {
                    childinode.block[i]= -1;
                }

                for(int i=0;i<INODE_NUMBER;i++)
                {
                    if( block_status(i,inodebitmap) == 0 )
                    {
                        fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
                        fwrite(&childinode, sizeof (struct inode), 1, diskfd);
                        fflush(diskfd);
                        toggle_bit(i,inodebitmap);
                        return 0;
                    }
                }
            }
        }
    }

    return -1;
}
int fs_Unlink(int pinum, char *name)
{
    if( pinum < 0 || pinum >= INODE_NUMBER )
        return  -1;

    if( block_status(pinum, inodebitmap) != 0 )
    {
        struct inode pinode;
        memset( &pinode,0,sizeof (struct inode));
        fseek(diskfd, pinum*sizeof(struct inode),SEEK_SET);
        fread(&pinode, sizeof (struct inode), 1, diskfd);
        if(  pinode.pinum >= 0)
        {
            if(pinode.type  == MFS_DIRECTORY )
            {
                struct inode childinode;
                memset( &childinode,0,sizeof (struct inode));
                for(int i=0;i<INODE_NUMBER;i++)
                {
                    if( block_status(i,inodebitmap) != 0 )
                    {
                        fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
                        fread(&childinode, sizeof (struct inode), 1, diskfd);

                        if(  childinode.pinum == pinum && strcmp(name,childinode.name )==0)
                        {
                            if(childinode.type  == MFS_DIRECTORY )
                            {
                                for(int child=0;child<INODE_NUMBER;child++)
                                {
                                    if( block_status(child,inodebitmap) != 0 )
                                    {
                                        struct inode findFile;
                                        memset( &findFile,0,sizeof (struct inode));
                                        fseek(diskfd, child*sizeof(struct inode),SEEK_SET);
                                        fread(&findFile, sizeof (struct inode), 1, diskfd);
                                        if(  findFile.pinum == i )
                                            return -1;
                                    }
                                }
                                childinode.blockcount=0;
                                childinode.size=0;
                                childinode.pinum = -1;
                                toggle_bit(i,inodebitmap);
                                fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
                                fwrite(&childinode, sizeof (struct inode), 1, diskfd);
                                fflush(diskfd);
                            }
                            else
                            {
                                for( int j = 0; j < FILE_MAX_BLOCK_NUM ; j ++)
                                {
                                    if( childinode.block[j] != -1 )
                                        toggle_bit(childinode.block[j],blockbitmap);
                                }
                                childinode.blockcount=0;
                                childinode.size=0;
                                childinode.pinum = -1;
                                toggle_bit(i,inodebitmap);
                                fseek(diskfd, i*sizeof(struct inode),SEEK_SET);
                                fwrite(&childinode, sizeof (struct inode), 1, diskfd);
                                fflush(diskfd);
                            }
                            return 0;
                        }
                    }
                }
                return 0;
            }
        }
    }
    return  -1;
}

int fs_close()
{
    fclose(diskfd);
    return 0;
}

