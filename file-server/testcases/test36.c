#include "mfs.h"
#include <stdlib.h>
#include <string.h>

//
// MFS_Unlink: name is not existed
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  // /dir1
  int rc;

  rc = MFS_Unlink(0, "dir1");
  if (rc == -1) return -1;

  return 0;
}
