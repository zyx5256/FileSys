#include "mfs.h"
#include <stdlib.h>
#include <string.h>

//
// MFS_Unlink: a file
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  // /dir1
  int rc;
  rc = MFS_Creat(0, MFS_REGULAR_FILE, "whatever");
  if (rc == -1) return -1;

  int inum = MFS_Lookup(0, "whatever");
  if (inum <=0) return -1;

  
  rc = MFS_Unlink(0, "whatever");
  if (rc == -1) return -1;

  rc = MFS_Lookup(0, "whatever");
  if (rc >= 0) return -1;

  MFS_Stat_t m;
  rc = MFS_Stat(inum, &m);
  if (rc == 0) return -1;
  return 0;
}
