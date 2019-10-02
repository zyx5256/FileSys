#include "mfs.h"
#include <stdlib.h>
#include <stdio.h>
//
// MFS_Creat: create a new dir
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  int rc;
  rc = MFS_Creat(0, MFS_DIRECTORY, "dir0");
  if (rc == -1) return -1;

  int inum = MFS_Lookup(0, "dir0");
  if (inum <= 0) return -1;

  MFS_Stat_t m;
  rc = MFS_Stat(inum, &m);
  if (rc == -1) return -1;
  
  printf("m.size = %d\n", m.size);

  if (m.size == 2 * sizeof(MFS_DirEnt_t) && m.type == MFS_DIRECTORY &&
      m.blocks == 1) 
    return 0;
  else 
    return -1;
      
  return 0;
}
