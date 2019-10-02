#include "mfs.h"
#include <stdlib.h>
//
// MFS_Creat: invalid num
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));
  
  int rc;
  rc = MFS_Creat(-1, MFS_DIRECTORY, "dir0");
  if (rc == 0) return -1;

  rc = MFS_Creat(4096, MFS_REGULAR_FILE, "file0");
  if (rc == 0) return -1;

  rc = MFS_Creat(1, MFS_REGULAR_FILE, "file0");
  if (rc == 0) return -1;

  return 0;
}
