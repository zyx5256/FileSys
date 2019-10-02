#include "mfs.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));
  MFS_Stat_t stat;

  if (MFS_Creat(0, MFS_REGULAR_FILE, "file.txt") == -1)
	  return -1;

  int inode = MFS_Lookup(0, "file.txt");
  if (inode == -1 || inode == 0)
	  return -1;

  char buf[MFS_BLOCK_SIZE];
  memset(buf, 99, MFS_BLOCK_SIZE);
  if (MFS_Write(inode, buf, 0) == -1)
	  return -1;

  if (MFS_Stat(inode, &stat) != 0)
	  return -1;
  if (stat.type != MFS_REGULAR_FILE)
	  return -1;
  if (stat.size !=  MFS_BLOCK_SIZE)
	  return -1;
  if (stat.blocks != 1)
	  return -1;

  return 0;

}

