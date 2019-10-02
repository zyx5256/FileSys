#include "mfs.h"
#include <stdlib.h>
#include <string.h>

//
// MFS_Creat: create an existed dir
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  int rc;
  rc = MFS_Creat(0, MFS_DIRECTORY, "dir0");
  if (rc == -1) return -1;

  int inum = MFS_Lookup(0, "dir0");
  if (inum <= 0) return -1;

  MFS_Stat_t m1, m2;
  rc = MFS_Stat(inum, &m1);
  if (rc == -1) return -1;

  rc = MFS_Creat(0, MFS_DIRECTORY, "dir0");
  if (rc == -1) return -1;

  int inum2 = MFS_Lookup(0, "dir0");
  if (inum2 <= 0) return -1;
  if (inum2 != inum) return -1;

  rc = MFS_Stat(inum2, &m2);
  if (rc == -1) return -1;
  
  if (m1.size == m2.size && m1.type == m2.type && m1.blocks == m2.blocks)
    return 0;
  else 
    return -1;
}
