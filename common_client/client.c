#include "interfaces.h"

static int glb_client_fd;
static int glb_server_fd;

int glb_epoll_fd = 0;


void epoll_Creat(void)
{

    glb_epoll_fd = epoll_create(MAX_EPOLL_EVENTS);
    if(glb_epoll_fd < 0)
    {
        printf("error when call epoll_create()\n");
        return ;
    }
    printf("create epoll for client success \n ");
}

static void epoll_AddEvents(int fd, int event)
{

    printf(" add events %d for fd %d !\n", event, fd);
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.events = event;
    ev.data.ptr = NULL;
    ev.data.fd = fd;
    epoll_ctl(glb_epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    printf(" Add events %d for fd %d success !\n", event, fd);
}

// Used to handle the client connect event
static void*  EventLoopThread
(
    void* objPtr
)
{
    printf("We are now in a client event loop thread\n");

    struct epoll_event epollEventList[MAX_EPOLL_EVENTS];

    int waitNum = 0;
    while(1)
    {
        waitNum = epoll_wait(glb_epoll_fd, epollEventList, MAX_EPOLL_EVENTS, -1);

        if (waitNum > 0)
        {
            int i = 0;
            for(i=0; i<waitNum; i++)
            {
                printf("waitnum is %d, fd:%d is ready, the event is %d\n",waitNum, epollEventList[i].data.fd, epollEventList[i].events);
                if(glb_server_fd == epollEventList[i].data.fd)
                {
                    // when success received the data, we need to send info to client;
                    client_SendInfo(glb_server_fd);
                }
                else if(glb_client_fd == epollEventList[i].data.fd)
                {
                    client_ReceivedInfo(epollEventList[i].data.fd);
                }
            }
        }
        else if (waitNum < 0)
        {
                printf("epoll_wait() failed!\n");
        }
        else
        {
            printf("epoll_wait() returned zero!\n");
        }
    }
    return NULL;
}

static void creat_EventLoopThread(void)
{
    pthread_t  threadHandle;
    pthread_create(&threadHandle, NULL, EventLoopThread, NULL);
}

int main(void)
{
    epoll_Creat();

    glb_client_fd = client_Connect(FIFO_S2C_NAME, O_RDONLY);
    glb_server_fd = client_Connect(FIFO_C2S_NAME, O_WRONLY);
    printf("the glb_server_fd is %d,  glb_client_fd is %d\n", glb_server_fd, glb_client_fd);
    epoll_AddEvents(glb_client_fd, EPOLLIN | EPOLLOUT);
    epoll_AddEvents(glb_server_fd, EPOLLIN | EPOLLOUT);

    creat_EventLoopThread();
    while(1);

}

