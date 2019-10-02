#include "mfs.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));
	char* dirs[] = {"dir0", "dir1", "dir2", "dir3", "dir4", "dir5", "dir6",
		"dir7", "dir8", "dir9", "dir10", "dir11", "dir12", "dir13", ".", ".."};
	int inodes[16];	
	int i;
	for (i = 0; i < 14; i++) {
  	if (MFS_Creat(0, MFS_DIRECTORY, dirs[i]) == -1)	
	  	return -1;
		inodes[i] = MFS_Lookup(0, dirs[i]);
		if (inodes[i] <= 0) return -1;
	}
	inodes[14] = MFS_Lookup(0, ".");
	if (inodes[14] < 0) return -1;
	inodes[15] = MFS_Lookup(0, "..");
	if (inodes[15] < 0) return -1;

	// double check if any inode num is reused
	int j;
	for (i = 0; i < 13; i++)
		for (j = i+1; j < 14; j++)
		if (inodes[i] == inodes[j]) return -1;

	// reread the dir block	
	
  char buf[MFS_BLOCK_SIZE];
  memset(buf, 0, MFS_BLOCK_SIZE);
  if (MFS_Read(0, buf, 1) == 0)
	  return -1;
  if (MFS_Read(0, buf, 0) == -1)
	  return -1;
	
	// sanity check
	int b[16];
	char* dirs2[16];
	int inodes2[16];
	memset(b, 0, 16);
	MFS_DirEnt_t* e;
	for (i = 0; i < 16; i ++) {
		e = buf + i * sizeof(MFS_DirEnt_t);
		inodes2[i] = e->inum;
		dirs2[i] = strdup(e->name);
	}
 	for (i = 0; i < 16; i ++) {
		int fi = 0;
		int fn = 0;
		for (j = 0; j < 16; j++) 
			if (inodes[i] == inodes2[j]) {
				fi = 1;
				break;
			}
		for (j = 0; j < 16; j ++)
			if (strcmp(dirs[i], dirs2[j]) == 0) {
				fn = 1;
				break;
			}
			if (!fi || !fn) return -1;
	}
	
  return 0;

}

