#include "mfs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//
// MFS_Unlink: empty directory
//

int main(int argc, char* argv[]) {
  MFS_Init(argv[2], atoi(argv[1]));

  // /dir1
  int rc;
  rc = MFS_Creat(0, MFS_DIRECTORY, "dir1");
  if (rc == -1) return -1;

  int inum = MFS_Lookup(0, "dir1");
  if (inum <=0) return -1;
	printf("inum = %d\n", inum);
	
	rc = MFS_Creat(0, MFS_REGULAR_FILE, "whatever");
	if (rc == -1) return -1;
	
	int inum3 = MFS_Lookup(0, "whatever");
	printf("inum3 = %d\n", inum3);
	if (inum3 <= 0 || inum3 == inum) return -1;

	rc = MFS_Creat(0, MFS_DIRECTORY, "dir2");
	if (rc == -1) return -1;

	int inum2 = MFS_Lookup(0, "dir2");
	if (inum2 <=0 || inum2 == inum || inum3 == inum2) return -1;

	rc = MFS_Unlink(0, "whatever");
	if (rc == -1) return -1;

  rc = MFS_Unlink(0, "dir1");
  if (rc == -1) return -1;

  rc = MFS_Lookup(0, "dir1");
  if (rc >= 0) return -1;

	rc = MFS_Lookup(0, "dir2");
	if (rc < 0) return -1;
	
	MFS_Stat_t m;	  
  rc = MFS_Stat(inum, &m);
  if (rc == 0) return -1;

	rc = MFS_Stat(0, &m);
	if (rc == -1) return -1;
	printf("m.size = %d\n", m.size);

// sanity check
	if (m.size != 3 * sizeof(MFS_DirEnt_t)) return -1;
	char buf[MFS_BLOCK_SIZE];
	if ((rc = MFS_Read(0, buf, 0)) < 0) return -1;
	int i;
	MFS_DirEnt_t* e;
	int fd2, fc, fp;
	fd2 = fc = fp = 0;
	for (i = 0; i < 5; i++) {



		e = buf + i * sizeof(MFS_DirEnt_t);
		printf("e.inum = %d\n", e->inum);	
		if (e->inum >= 0) {
			if (strcmp(e->name, ".") == 0) fc = 1;
			if (strcmp(e->name, "..") == 0) fp = 1;
			if (strcmp(e->name, "dir2") == 0) fd2 = 1;
		}	
	}

	if (!fc || !fp || !fd2) return -1;

  return 0;
}
