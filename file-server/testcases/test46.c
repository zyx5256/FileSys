#include "mfs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//
// unlink should retry
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));
  
  if (MFS_Unlink(8, "dir") == 0) goto fail;
  
  printf("PASSED");
  return 0;
  
 fail:
  printf("FAILED");
  return -1;
}

