HOW TO RUN
==========

1. copy everything in this directory to your working directory

2. the test expects server name and port passed by MFS_SERVER and MFS_PORT
i.e you have to create these environment variables and set correct values.
For example: put the following lines in your bashrc, and source your bash file
export MFS_SERVER=localhost
export MFS_PORT=10003

3. to run all test at once, use

   $ ./run-test.py

   for run a specfific test, use

   $ ./run-test.py [test number (1-46)]

// ******************************************************* //
// * *                  IMPORTANT                          //
// * What if a particular test case have some problem??? * //
// ******************************************************* //
Well, in that case, PLEASE do the following:
1. see if testclient is compiled successfuly

2. check if your MFS_SERVER and MFS_PORT is configure correctly:
To do this:
$ echo $MFS_SERVER
(your server name should appear corectly)
$ echo $MFS_PORT

3. (THIS IS IMPORTANT, that help you debug your code, and program correctly)
Look in to the test code (i.e if you test test X, you should look into
./testcases/testx.c) and see what it realy does, and what it expects.
Try to write a small client that does similar thing and run the test
separately (w/o the test framework).


TEST OUTLINE
============

Basic
-----
1) - README or README.txt
2 - Makefile or makefile
3 - make generate server and libmfs.so


MFS interface
-------------
4- MFS_Lookup: invalid pinum
5- MFS_Lookup: name does not exists
6- MFS_Lookup: lookup . at ROOT
9- MFS_Lookup: a file

10- MFS_Stat: inum does not exist
11- MFS_Stat: for new dir
12- MFS_Stat: for dir having some entries
14- MFS_Stat: stat for non-empty file

15- MFS_Write: invalid inum
16- MFS_Write: invalid block
17- MFS_Write: not a regular file
18- MFS_Write: write single block

21- MFS_Write: overwrite a block
22- MFS_Read: invalid num
23- MFS_Read: invalid block
24- MFS_Read: invalid block
25- MFS_Read: invalid block
26- MFS_Read: a block in directory

28- MFS_Creat: invalid pinum
29- MFS_Creat: creat new a directory
30- MFS_Creat: creat new a file
31- MFS_Creat: creat existed file
32- MFS_Creat: creat existed directory

34- MFS_Unlink: invalid pinum
36- MFS_Unlink: name is not existed
37- MFS_Unlink: an empty directory
38- MFS_Unlink: a file

Server: special case
------
40
- Server: startup non-existing fsimage, see if it creates the image correctly
- Server: startup with existing fsiamge, double check it it load it correctly

Client timout
-------------
- Check if client timeout and retry correctly with each MFS_XYZ
41+ lookup
42+ write
43+ read
44+ stat
45+ creat
46+ unlink
