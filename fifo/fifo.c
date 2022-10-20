

#include "interfaces.h"


void client_ReceivedInfo(int fd)
{
    char recvbuff[256]={0};

    read(fd, recvbuff, sizeof(recvbuff));
    printf("client Received info from server:%s\n",recvbuff);
    printf("%s %d\n",__FUNCTION__,__LINE__);
}

void client_SendInfo(int fd)
{
    char sndbuff[256]={0};
    strcpy(sndbuff, "I am client of fifo, sending info to server");
    int len = strlen(sndbuff);
    write(fd, sndbuff, len);
    printf("send data to server \n");
}

int client_Connect(char* path, int flags)
{
    int fd = -1;
    printf(" open client fd\n");

    if(access(path, F_OK)==  -1)
    {
        if(mkfifo(path, 0777) < 0)
        {
            printf("create fifo for client fail!\n");
            return -1;
        }
    }
    fd = open(path, flags);
    if (fd < 0)
    {
        printf("open client fd error !:%d\n",fd);
        printf("%s %d\n",__FUNCTION__,__LINE__);
    }
    printf(" Open client fd success !\n");
    return fd;

}

void server_ReceivedInfo(int fd)
{
    char recvbuff[256]={0};

    read(fd, recvbuff, sizeof(recvbuff));
    printf("server Received info from client:%s\n",recvbuff);
    printf("%s %d\n",__FUNCTION__,__LINE__);
}

void server_SendInfo(int fd)
{
    char sndbuff[256]={0};
    static uint16_t i = 0 ;
    strcpy(sndbuff, "I am server of fifo, sending info to client");
    int len = strlen(sndbuff);
    write(fd, sndbuff, len);
    printf("send data to client for %d times\n", i);
    i ++;
    if (i> 1000)
    {
        i = 0;
    }
}

int server_Connect(char* path, int flags)
{
    int fd = -1;
    printf(" open server fd\n");
    if(access(path, F_OK)== -1)
    {
        if(mkfifo(path, 0777) < 0)
        {
            printf("create fifo for server fail!\n");
            printf("%s %d\n",__FUNCTION__,__LINE__);
            return -1;
        }
    }
    fd = open(path, flags);

    if (fd < 0)
    {
        printf("open server fd error !:%d\n",fd);
        printf("%s %d\n",__FUNCTION__,__LINE__);
        return -1;
    }
    printf(" open server fd success !\n");
    return fd;
}


