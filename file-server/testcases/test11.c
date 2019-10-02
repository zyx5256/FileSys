#include "mfs.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));
  MFS_Stat_t stat;

  if (MFS_Creat(0, MFS_DIRECTORY, "usr") == -1)  
	  return -1;

  int inode = MFS_Lookup(0, "usr");
  if (inode == -1)
	  return -1;
  if (MFS_Stat(inode, &stat) != 0)
	  return -1;
  if (stat.type != MFS_DIRECTORY)
	  return -1;
  if (stat.size !=  2 * sizeof(MFS_DirEnt_t))
	  return -1;
  if (stat.blocks != 1)
	  return -1;
  return 0;

}

