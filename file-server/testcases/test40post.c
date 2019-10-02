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
  
  

  int rc = MFS_Lookup(0, "dir0");
  printf("status from lookup = %d\n", rc);
  if (rc == -1) return -1;

  int inum = MFS_Lookup(0, "file0");
  if (inum <= 0) return -1;

  MFS_Stat_t m;
  rc = MFS_Stat(inum, &m);
  if (rc == -1) return -1;
  if (m.size != 2 * MFS_BLOCK_SIZE || 
      m.blocks != 2 || m.type != MFS_REGULAR_FILE) {
    printf("m.size = %d\n", m.size);
    printf("m.types = %d\n", m.type);
    printf("m.blocks = %d\n", m.blocks);
    printf("Stat test failed");
    return -1;
  }

  char buf[MFS_BLOCK_SIZE];
  memset(buf, 0, MFS_BLOCK_SIZE);
  rc = MFS_Read(inum, buf, 0);
  if (rc == -1) return -1;

  int i;
  for (i = 0; i < MFS_BLOCK_SIZE; i++)
    if (buf[i] != 'A') return -1;

  rc = MFS_Read(inum, buf, 1);
  if (rc == -1) return -1;
  for (i = 0; i < MFS_BLOCK_SIZE; i++)
    if (buf[i] != 'B') return -1;

  return 0;    
}
