#include "mfs.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  if (MFS_Lookup(0, ".") != 0)
	  return -1;
  return 0;

}

