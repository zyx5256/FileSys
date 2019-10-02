#include <stdio.h>
#include <stdlib.h>
#include "mfs.h"

//
// server with existing/non-existing fsimage
//

/*
 /dir0
 /file0
 */

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  int rc = MFS_Creat(0, MFS_DIRECTORY, "dir0");
  if (rc == -1) return -1;

  rc = MFS_Creat(0, MFS_REGULAR_FILE, "file0");
  if (rc == -1) return -1;
  
  int inum = MFS_Lookup(0, "file0");
  if (inum <= 0) return -1;

  char buf[MFS_BLOCK_SIZE];
  memset(buf, 'A', MFS_BLOCK_SIZE);
  
  rc = MFS_Write(inum, buf, 0);
  if (rc == -1) return -1;
  memset(buf, 'B', MFS_BLOCK_SIZE);
  rc = MFS_Write(inum, buf, 1);
  if (rc == -1) return -1;
  return 0;    
}
