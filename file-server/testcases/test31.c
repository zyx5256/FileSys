#include "mfs.h"
#include <stdlib.h>
#include <string.h>
//
// MFS_Creat: create an existed file
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  int rc;
  rc = MFS_Creat(0, MFS_REGULAR_FILE, "file0");
  if (rc == -1) return -1;


  int inum = MFS_Lookup(0, "file0");
  if (inum <= 0) return -1;


  char buffer[MFS_BLOCK_SIZE];
  memset(buffer, 99, MFS_BLOCK_SIZE);
  rc = MFS_Write(inum, buffer, 0);

  MFS_Stat_t m1, m2;
  rc = MFS_Stat(inum, &m1);
  if (rc == -1) return -1;


  rc = MFS_Creat(0, MFS_REGULAR_FILE, "file0");
  if (rc == -1) return -1;

  int inum2 = MFS_Lookup(0, "file0");
  if (inum2 <= 0) return -1;
  if (inum2 != inum) return -1;

  rc = MFS_Stat(inum2, &m2);
  if (rc == -1) return -1;
  
  if (m1.size == m2.size && m1.type == m2.type && m1.blocks == m2.blocks)
    return 0;
  else 
    return -1;
}
