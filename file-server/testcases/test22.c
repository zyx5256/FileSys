#include "mfs.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  char buf[MFS_BLOCK_SIZE];
  memset(buf, 99, MFS_BLOCK_SIZE);
  if (MFS_Read(-1, buf, 0) == 0)
	  return -1;
  if (MFS_Read(1, buf, 0) == 0)
	  return -1;
  if (MFS_Read(4096, buf, 0) == 0)
	  return -1;
  return 0;

}

