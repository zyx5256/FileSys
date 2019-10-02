#include <stdio.h>
#include "mfs.h"
#include "udp.h"
#include "msg.h"

static int sd;
static struct sockaddr_in addr;

#define TRYNUMBER 2;
int MFS_Init(char *hostname, int port)
{
    sd = UDP_Open(13299); //communicate through specified port
    assert(sd > -1);

    int rc = UDP_FillSockAddr(&addr, hostname, port); //contact server at specified port
    assert(rc == 0);
    return 0;
}

int MFS_Lookup(int pinum, char *name)
{
    int trytime=TRYNUMBER;
    while( trytime-- ) {
        mfsmsg lookupmsg;
        memset(&lookupmsg,0,sizeof (lookupmsg));
        lookupmsg.type = MSG_LOOKUP_REQ;
        memcpy(lookupmsg.data,&pinum,sizeof (pinum) );
        strcpy(lookupmsg.data + sizeof (pinum),name );

        struct sockaddr_in addr2;
        int rc = UDP_Write(sd, &addr,(char *)&lookupmsg, sizeof (lookupmsg));
        printf("MFS_Lookup CLIENT=======>SERVER:: sent message (%d)\n", rc);
        printf("MFS_Lookup pinum  %d name %s \n", pinum, name);
        if (rc > 0) {
            mfsmsgresp resp;
            memset(&resp,0,sizeof (resp));
            resp.result=-1;

            resp.result=-1;

            printf("try %d \n", trytime +1);
            rc = UDP_Read(sd, &addr2, (char *)&resp, sizeof (resp)  );
            printf("result rc %d \n",rc);

            if( rc <= 0 )
                continue;
            printf("MFS_Lookup CLIENT<======SERVER:: read message %d bytes (message: ' %d %d ')\n", rc, resp.type,resp.result);
            return resp.result;
        }
    }
    return -1;
}
int MFS_Stat(int inum, MFS_Stat_t *m)
{
    int trytime=TRYNUMBER;
    while( trytime-- ) {
        mfsmsg msgreq;
        memset(&msgreq,0,sizeof (msgreq));
        msgreq.type = MSG_STAT_REQ;

        memcpy(msgreq.data,&inum,sizeof (inum) );

        struct sockaddr_in  addr2;
        int rc = UDP_Write(sd, &addr, (char *)&msgreq, sizeof (msgreq));
        printf("MFS_Stat CLIENT=======>SERVER:: sent message (%d)\n", rc);
        printf("MFS_Stat inum  %d   \n", inum);
        if (rc > 0) {
            mfsmsgresp resp;
            memset(&resp,0,sizeof (resp));
            resp.result=-1;

            printf("try %d \n", trytime +1);
            rc = UDP_Read(sd, &addr2, (char *)&resp, sizeof (resp)  );
            printf("result rc %d \n",rc);

            if( rc <= 0 )
                continue;

            printf("MFS_Stat CLIENT<======SERVER:: read message %d bytes (message: ' %d %d ')\n", rc, resp.type,resp.result);

            memcpy(m,resp.data,sizeof (MFS_Stat_t) );
            printf("data: MFS_Stat_t: size %d type %d blocks %d \n", m->size,m->type,m->blocks);

            return resp.result;
        }
        else {
            printf("result rc %d \n",rc);
            break;
        }
    }
    return -1;
}
int MFS_Write(int inum, char *buffer, int block)
{   int trytime=TRYNUMBER;
    while( trytime-- ) {
        mfsmsg msgreq;
        memset(&msgreq,0,sizeof (msgreq));
        msgreq.type = MSG_WRITE_REQ;

        memcpy(msgreq.data,&inum,sizeof (inum) );
        memcpy(msgreq.data +sizeof (inum) ,&block,sizeof (inum) );
        memcpy(msgreq.data +sizeof (inum)+sizeof (block),buffer,MFS_BLOCK_SIZE );

        struct sockaddr_in  addr2;
        int rc = UDP_Write(sd, &addr, (char *)&msgreq, sizeof (msgreq));
        printf("MFS_Write CLIENT=======>SERVER:: sent message (%d)\n", rc);
        printf("MFS_Write inum  %d block %d \n", inum, block);
        if (rc > 0) {
            mfsmsgresp resp;
            memset(&resp,0,sizeof (resp));
            resp.result=-1;

            printf("try %d \n", trytime +1);
            rc = UDP_Read(sd, &addr2, (char *)&resp, sizeof (resp)  );
            printf("result rc %d \n",rc);

            if( rc <= 0 )
                continue;
            printf("MFS_Write CLIENT<======SERVER:: read message %d bytes (message: ' %d %d ')\n", rc, resp.type,resp.result);
            return resp.result;
        }
        else {
            printf("result rc %d \n",rc);
            break;
        }
    }
    return -1;
}
int MFS_Read(int inum, char *buffer, int block)
{
    int trytime=TRYNUMBER;
    while( trytime-- ) {
        mfsmsg msgreq;
        memset(&msgreq,0,sizeof (msgreq));
        msgreq.type = MSG_READ_REQ;

        memcpy(msgreq.data,&inum,sizeof (inum) );
        memcpy(msgreq.data +sizeof (inum) ,&block,sizeof (inum) );

        struct sockaddr_in  addr2;
        int rc = UDP_Write(sd, &addr, (char *)&msgreq, sizeof (msgreq));
        printf("MFS_Read CLIENT=======>SERVER:: sent message (%d)\n", rc);
        printf("MFS_Read inum  %d block %d   \n", inum, block );
        if (rc > 0) {
            mfsmsgresp resp;
            memset(&resp,0,sizeof (resp));
            resp.result=-1;

            printf("try %d \n", trytime +1);
            rc = UDP_Read(sd, &addr2, (char *)&resp, sizeof (resp)  );
            printf("result rc %d \n",rc);

            if( rc <= 0 )
                continue;
            printf("MFS_Read CLIENT<======SERVER:: read message %d bytes (message: ' %d %d ')\n", rc, resp.type,resp.result);

            //printf("read: %s \n", resp.data);

            memcpy( buffer,resp.data,MFS_BLOCK_SIZE );

            return resp.result;
        }
        else {
            printf("result rc %d \n",rc);
            break;
        }
    }
    return -1;
}
int MFS_Creat(int pinum, int type, char *name)
{
    int trytime=TRYNUMBER;
    while( trytime-- ) {
        mfsmsg msgreq;
        memset(&msgreq,0,sizeof (msgreq));
        msgreq.type = MSG_CREATE_REQ;

        memcpy(msgreq.data,&pinum,sizeof (pinum) );
        memcpy(msgreq.data +sizeof (pinum) ,&type,sizeof (type) );
        strcpy(msgreq.data +sizeof (pinum)+sizeof (type),name );

        struct sockaddr_in  addr2;
        int rc = UDP_Write(sd, &addr,(char *) &msgreq, sizeof (msgreq));
        printf("MFS_Creat CLIENT=======>SERVER:: sent message (%d)\n", rc);
        printf("MFS_Creat pinum %d type %d name %s \n", pinum,type, name);
        if (rc > 0) {
            mfsmsgresp resp;
            memset(&resp,0,sizeof (resp));
            resp.result=-1;

            printf("try %d \n", trytime +1);
            rc = UDP_Read(sd, &addr2, (char *)&resp, sizeof (resp)  );
            printf("result rc %d \n",rc);

            if( rc <= 0 )
                continue;
            printf("MFS_Creat CLIENT<======SERVER:: read message %d bytes (message: ' %d %d ')\n", rc, resp.type,resp.result);
            return resp.result;
        }
        else {
            printf("result rc %d \n",rc);
            break;
        }
    }
    return -1;

}
int MFS_Unlink(int pinum, char *name)
{
    int trytime=TRYNUMBER;
    while( trytime-- ) {
        mfsmsg msgreq;
        memset(&msgreq,0,sizeof (msgreq));
        msgreq.type = MSG_UNLINK_REQ;

        memcpy(msgreq.data,&pinum,sizeof (pinum) );
        strcpy(msgreq.data +sizeof (pinum),name );

        struct sockaddr_in  addr2;
        int rc = UDP_Write(sd, &addr, (char *)&msgreq, sizeof (msgreq));
        printf("MFS_Unlink  CLIENT=======>SERVER:: sent message (%d)\n", rc);
        printf("MFS_Unlink pinum %d name %s \n", pinum, name);
        if (rc > 0) {
            mfsmsgresp resp;
            memset(&resp,0,sizeof (resp));
            resp.result=-1;

            printf("try %d \n", trytime +1);
            rc = UDP_Read(sd, &addr2, (char *)&resp, sizeof (resp)  );
            printf("result rc %d \n",rc);
            if( rc <= 0 )
                continue;

            printf("MFS_Unlink  CLIENT<======SERVER:: read message %d bytes (message: ' %d %d ')\n", rc, resp.type,resp.result);
            return resp.result;
        }
        else {
            printf("result rc %d \n",rc);
            break;
        }
    }
    return -1;
}
