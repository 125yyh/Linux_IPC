


void client_RecvInfo(int fd)
{
    char recvbuff[256]={0};

    recv(fd, recvbuff, 256, 0);
    printf("client Received info from server:%s\n",recvbuff);
    printf("%s %d\n",__FUNCTION__,__LINE__);
}

void client_SendInfo(int fd)
{
    char sndbuff[256]={0};
    strcpy(sndbuff, "I am client, sending info to server");
    int len = strlen(sndbuff);
    send(fd, sndbuff, len, 0);
    printf("send data to server \n");
}

void server_RecvInfo(int fd)
{
    char recvbuff[256]={0};

    recv(fd, recvbuff, 256, 0);
    printf("client Received info from server:%s\n",recvbuff);
    printf("%s %d\n",__FUNCTION__,__LINE__);
}

void server_SendInfo(int fd)
{
    char sndbuff[256]={0};
    strcpy(sndbuff, "I am client, sending info to server");
    int len = strlen(sndbuff);
    send(fd, sndbuff, len, 0);
    printf("send data to server \n");
}


