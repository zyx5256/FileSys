#include <stdio.h>
#include "udp.h"
#include "fs.h"

#include "msg.h"
//#define BUFFER_SIZE (4096)

int main(int argc, char *argv[])
{ 
    if(argc<3)
    {
        printf("Usage: server [portnum] [file-system-image]\n");
        exit(1);
    }

    printf("use portnum:          %s\n",argv[1]);
    printf("use file-system-image:%s n",argv[2]);

    if( fs_init(argv[2]) != 0 )
    {
        printf("file server init failed!!!\n");
        exit(1);
    }

    int portid = atoi(argv[1]);
    assert(portid > 0);
    int sd = UDP_Open(portid);
    assert(sd > -1);

    printf("waiting in loop, wait for client connect!!!\n");

    while (1) {
        struct sockaddr_in s;
        char databuffer[sizeof (mfsmsg)];
        memset(databuffer,0,sizeof (mfsmsg));
        int rc = UDP_Read(sd, &s, databuffer, sizeof (mfsmsg));
        if (rc > 0)
        {
            mfsmsg *datamfsmsg = (mfsmsg *)databuffer;
            printf("client============>>server read %d bytes, msg.type %d\n", rc, datamfsmsg->type);
            if( datamfsmsg->type == MSG_LOOKUP_REQ)
            {
                int pinum;
                memcpy(&pinum, datamfsmsg->data,sizeof (pinum) );
                char *name = datamfsmsg->data + sizeof (pinum);
                printf("====lookup req:  pinum %d, name %s\n", pinum, name);
                mfsmsgresp resultresp;
                resultresp.type=MSG_LOOKUP_RESP;
                resultresp.result = (short)fs_Lookup(pinum,name);
                printf("client<<<<============server  lookup resp: %d, %d \n", resultresp.type,resultresp.result);
                rc = UDP_Write(sd, &s, (char *)&resultresp, sizeof (resultresp));
            }
            else if( datamfsmsg->type == MSG_STAT_REQ)
            {
                int inum;
                memcpy(&inum, datamfsmsg->data,sizeof (inum) );
                printf("=====stat req:  inum %d \n", inum );
                MFS_Stat_t stat_t;
                mfsmsgresp resultresp;
                resultresp.type=MSG_STAT_RESP;
                resultresp.result = (short)fs_Stat(inum,&stat_t);
                memcpy(resultresp.data,&stat_t,sizeof (stat_t));
                printf("client<<<<============server  stat resp: %d, %d \n", resultresp.type,resultresp.result);
                rc = UDP_Write(sd, &s, (char *)&resultresp, sizeof (resultresp));
            } else if( datamfsmsg->type == MSG_WRITE_REQ)
            {
                int inum;
                memcpy(&inum, datamfsmsg->data,sizeof (inum) );

                int block;
                memcpy(&block, datamfsmsg->data + sizeof (inum) ,sizeof (block) );

                char *buffer = datamfsmsg->data + sizeof (inum) +sizeof (block) ;
                printf("=====write req:  inum %d block %d \n", inum ,block );
                // printf("=====write req:  buffer %s \n", buffer );
                mfsmsgresp resultresp;
                resultresp.type=MSG_WRITE_RESP;
                resultresp.result = (short)fs_Write(inum,buffer, block);
                printf("client<<<<============server  write resp: %d, %d \n", resultresp.type,resultresp.result);
                rc = UDP_Write(sd, &s,(char *) &resultresp, sizeof (resultresp));


            } else if( datamfsmsg->type == MSG_READ_REQ)
            {
                int inum;
                memcpy(&inum, datamfsmsg->data,sizeof (inum) );
                int block;
                memcpy(&block, datamfsmsg->data + sizeof (inum) ,sizeof (block) );
                printf("=====read req:  inum %d block %d \n", inum ,block );
                mfsmsgresp resultresp;
                resultresp.type=MSG_READ_RESP;
                resultresp.result= (short)fs_Read(inum,resultresp.data, block);
                printf("client<<<<============server  read resp: %d, %d \n", resultresp.type,resultresp.result);
                rc = UDP_Write(sd, &s, (char *)&resultresp, sizeof (resultresp));

            } else if( datamfsmsg->type == MSG_CREATE_REQ)
            {
                int pinum;
                memcpy(&pinum, datamfsmsg->data,sizeof (pinum) );
                int type;
                memcpy(&type, datamfsmsg->data + sizeof (pinum) ,sizeof (type) );
                char *name = datamfsmsg->data + sizeof (pinum) +sizeof (type) ;
                printf("=====create req:  pinum %d type %d name %s \n", pinum ,type,name );
                mfsmsgresp resultresp;
                resultresp.type=MSG_CREATE_RESP;
                resultresp.result = (short)fs_create(pinum,type, name);
                printf("client<<<<============server  create resp: %d, %d \n", resultresp.type,resultresp.result);
                rc = UDP_Write(sd, &s, (char *)&resultresp, sizeof (resultresp));

            } else if( datamfsmsg->type == MSG_UNLINK_REQ)
            {
                int pinum;
                memcpy(&pinum, datamfsmsg->data,sizeof (pinum) );
                char *name = datamfsmsg->data + sizeof (pinum);
                printf("=====unlink req:  pinum %d  name %s \n", pinum ,name );
                mfsmsgresp resultresp;
                resultresp.type=MSG_UNLINK_RESP;
                resultresp.result = (short)fs_Unlink(pinum,name);
                printf("client<<<<============server  unlink resp: %d, %d \n", resultresp.type,resultresp.result);
                rc = UDP_Write(sd, &s,(char *) &resultresp, sizeof (resultresp));
            }
        }
    }
    UDP_Close(sd);
    fs_close();
    return 0;
}
