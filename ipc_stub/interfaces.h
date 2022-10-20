/*
 * interfaces.h
 *
 */

#ifndef IPC_INTERFACE_INCLUDE_GUARD
#define IPC_INTERFACE_INCLUDE_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>

#define FIFO_S2C_NAME "./fifo_s2c_test"
#define FIFO_C2S_NAME "./fifo_c2s_test"
#define MAX_EPOLL_EVENTS 32

void client_ReceivedInfo(int fd);

void client_SendInfo(int fd);

/****************************************************************************************************
 *  client to connect ipc channel
 *
 *@input
 *   --path[IN]  : The ipc channel file path
 *   --flags[IN] : The flag to open the path, such as :O_RDONLY, O_WRONLY, O_RDWR
 *@output
 *   --NULL
 *@return
 *  -- The fd of the open file.
 *
 ***************************************************************************************************/
int client_Connect(char* path, int flags);

void server_ReceivedInfo(int fd);

void server_SendInfo(int fd);

/****************************************************************************************************
 *  client to connect ipc channel
 *
 *@input
 *   --path[IN]  : The ipc channel file path
 *   --flags[IN] : The flag to open the path, such as :O_RDONLY, O_WRONLY, O_RDWR
 *@output
 *   --NULL
 *@return
 *  -- The fd of the open file.
 *
 ***************************************************************************************************/
int server_Connect(char* path, int flags);


#endif //IPC_INTERFACE_INCLUDE_GUARD
