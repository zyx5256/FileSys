#include "mfs.h"
#include <stdlib.h>
//
// MFS_Creat: create a new file
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  int rc;
  rc = MFS_Creat(0, MFS_REGULAR_FILE, "file0");
  if (rc == -1) return -1;

  int inum = MFS_Lookup(0, "file0");
  if (inum <= 0) return -1;

  MFS_Stat_t m;
  rc = MFS_Stat(inum, &m);
  if (rc == -1) return -1;

  if (m.size == 2 * 0 && m.type == MFS_REGULAR_FILE &&  m.blocks == 0) 
    return 0;
  else 
    return -1;
      
  return 0;
}
