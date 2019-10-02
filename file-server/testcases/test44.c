#include "mfs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//
// read should retry
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));
  char buf[MFS_BLOCK_SIZE];
  if (MFS_Read(0, buf, 0) == -1) goto fail;
  MFS_Stat_t m;
  int rc = MFS_Stat(0, &m);
  if (rc == -1) goto fail;

  int loop = m.size / sizeof(MFS_DirEnt_t);
  printf("loop = %d", loop);
  if (loop != 2) goto fail;
  
  int cur = 0;
  int par = 0;
  int i;
  MFS_DirEnt_t* e;
  for (i = 0; i < loop; i++) {
    e = buf + i * sizeof(MFS_DirEnt_t);
    printf("e->inum = %d\n", e->inum);
    printf("e->name = %s\n", e->name);
    if (e->inum == 0 && strcmp(".", e->name) == 0) cur = 1;
    if (e->inum == 0 && strcmp("..", e->name) == 0) par = 1;
  }

  printf("cur = %d\n", cur);
  printf("par = %d\n", par);
  if (!cur || !par) goto fail;
  
  printf("PASSED");
  return 0;
  
 fail:
  printf("FAILED");
  return -1;
}

