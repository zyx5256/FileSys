#include "mfs.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));
  MFS_Stat_t stat;
	
  if (MFS_Stat(-1, &stat) == 0)
	  return -1;
  if (MFS_Stat(1, &stat) == 0)
	  return -1;
  if (MFS_Stat(4096, &stat) == 0)
	  return -1;
  return 0;

}

