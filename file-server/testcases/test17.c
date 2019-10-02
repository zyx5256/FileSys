#include "mfs.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  if (MFS_Creat(0, MFS_DIRECTORY, "usr") == -1)
	  return -1;

  int inode = MFS_Lookup(0, "usr");
  if (inode == -1)
	  return -1;

  char buf[MFS_BLOCK_SIZE];
  memset(buf, 99, MFS_BLOCK_SIZE);
  if (MFS_Write(inode, buf, 0) == 0)
	  return -1;

  return 0;

}

