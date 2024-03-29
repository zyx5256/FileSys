#include "mfs.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  if (MFS_Creat(0, MFS_REGULAR_FILE, "file.txt") == -1)
	  return -1;

  int inode = MFS_Lookup(0, "file.txt");
  if (inode == -1)
	  return -1;

  char buf1[MFS_BLOCK_SIZE];
  memset(buf1, 99, MFS_BLOCK_SIZE);
  if (MFS_Write(inode, buf1, 0) == -1)
	  return -1;
  if (MFS_Write(inode, buf1, 1) == -1)
	  return -1;
  char result[MFS_BLOCK_SIZE];
  memset(result, 98, MFS_BLOCK_SIZE);
  if (MFS_Read(inode, result, 1) == -1)
	  return -1;
  if (memcmp(result, buf1, MFS_BLOCK_SIZE) != 0)
	  return -1;
  char buf2[MFS_BLOCK_SIZE];
  memset(buf2, 97, MFS_BLOCK_SIZE);
  if (MFS_Write(inode, buf2, 2) == -1)
	  return -1;

  memset(result, 98, MFS_BLOCK_SIZE);
  memset(result, 98, MFS_BLOCK_SIZE);
  if (MFS_Read(inode, result, 2) == -1)
	  return -1;
  if (memcmp(result, buf2, MFS_BLOCK_SIZE) != 0)
	  return -1;

  return 0;  

}

