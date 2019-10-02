#include "mfs.h"
#include <stdlib.h>
#include <unistd.h>
//
// Test timeout for lookup
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  int rc = MFS_Lookup(0, ".");
  if (MFS_Lookup(0, ".") != 0) {
    printf("FAILED");
    return -1;

  }
  printf("PASSED");
  return 0;
}

