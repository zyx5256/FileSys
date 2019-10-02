#include "mfs.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    MFS_Init(argv[2], atoi(argv[1]));


    return 0;
}


/*
{
    printf("file=%s,func=%s,line=%d\n",__FILE__,__FUNCTION__,__LINE__);
    {printf("file=%s,func=%s,line=%d\n",__FILE__,__FUNCTION__,__LINE__);{printf("file=%s,func=%s,line=%d\n",__FILE__,__FUNCTION__,__LINE__);{printf("file=%s,func=%s,line=%d\n",__FILE__,__FUNCTION__,__LINE__);{printf("file=%s,func=%s,line=%d\n",__FILE__,__FUNCTION__,__LINE__);return -1;}}}}
}*/
