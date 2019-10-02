#include "mfs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//
// write should retry
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  char buf[MFS_BLOCK_SIZE];
  memset(buf, 99, MFS_BLOCK_SIZE);
  if (MFS_Write(0, buf, 0) == 0) { // this should fail
    printf("FAILED");
    return -1;
  }

  printf("PASSED");
  return 0;
}

