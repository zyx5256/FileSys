#include "mfs.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  if (MFS_Lookup(0, "file.txt") != -1)
	  return -1;
  if (MFS_Creat(0, MFS_REGULAR_FILE, "file.txt") != 0)
	  return -1;
  if (MFS_Lookup(0, "file.txt") == -1)
	  return -1;
  return 0;

}

