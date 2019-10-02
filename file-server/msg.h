#ifndef __MSG_h__
#define __MSG_h__

#define BUFFER_SIZE (6400)

#define MSG_BASE 0

#define MSG_LOOKUP_REQ  MSG_BASE+1
#define MSG_LOOKUP_RESP MSG_BASE+2
#define MSG_STAT_REQ    MSG_BASE+3
#define MSG_STAT_RESP   MSG_BASE+4
#define MSG_WRITE_REQ   MSG_BASE+5
#define MSG_WRITE_RESP  MSG_BASE+6
#define MSG_READ_REQ    MSG_BASE+7
#define MSG_READ_RESP   MSG_BASE+8
#define MSG_CREATE_REQ  MSG_BASE+9
#define MSG_CREATE_RESP MSG_BASE+10
#define MSG_UNLINK_REQ  MSG_BASE+11
#define MSG_UNLINK_RESP MSG_BASE+12

typedef struct __mfsmsg{
    short type;
    char data[BUFFER_SIZE];
}mfsmsg;

typedef struct __mfsmsgresp{
    short type;
    short result;
    char data[BUFFER_SIZE];
}mfsmsgresp;

#endif

