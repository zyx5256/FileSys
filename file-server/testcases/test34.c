#include "mfs.h"
#include <stdlib.h>
#include <string.h>

//
// MFS_Unlink: invalid pinum
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  // /dir1
  int rc;
  rc = MFS_Unlink(1, "dir1");
  if (rc == 0) return -1;

  rc = MFS_Unlink(-1, "dir1");
  if (rc == 0) return -1;

  rc = MFS_Unlink(4096, "dir1");
  if (rc == 0) return -1;
  
  rc = MFS_Creat(0, MFS_REGULAR_FILE, "file0");
  if (rc == -1) return -1;

  int inum = MFS_Lookup(0, "file0");
  if (inum <=0) return -1;
  rc = MFS_Unlink(inum, "whatever");
  if (rc == 0) return -1;

  return 0;
}
