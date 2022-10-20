#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmdata.h"

#define SENDER_MSG "this is message just for performance test, no. "

int sem_init() {
    key_t key;
    key = ftok("/tmp",'a');
    if(key<0)
    {
         perror("ftok error");
         exit(1);
    }
    int semid = semget(key,1,IPC_CREAT|0666);
    if(semid<0)
    {
         perror("semget error");
         exit(1);
    }
    return semid;
}

int sem_set(int semid, int i) {
    int ret=semctl(semid,0,SETVAL,i);
    if(ret==-1)
    {
         perror("semctl error");
         exit(1);
    }
    return ret;
}
int sem_p(int semid) {
    struct sembuf P ={0,-1,SEM_UNDO};
    int ret=semop(semid,&P,1);
    if(ret==-1)
    {
         perror("semop P error");
         exit(1);
    }
    return 0;
}

int sem_v(int semid) {
    struct sembuf V ={0,1,SEM_UNDO};
    int ret=semop(semid,&V,1);
    if(ret==-1)
    {
         perror("semop unlock error");
         exit(1);
    }
    return ret;
}
int sem_del(int semid) {
    int ret=semctl(semid,0,IPC_RMID,0);
    if(ret==-1)
    {
          perror("semctl error");
          exit(1);
    }
    return ret;
}
int main(int argc, char *argv[])
{
    if (argc != 2 ) {
        printf("usage: ./server MSG_NUMBER\n");
        return 1;
    }
    int msg_number = atoi(argv[1]);

    int semid,ret;

    semid = sem_init();
    sem_set(semid, 0);


    int running = 1;
    void *shm = NULL;
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ + 1];//���ڱ���������ı�
    int shmid;
    //���������ڴ�
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if(shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
    shm = shmat(shmid, (void*)0, 0);
    if(shm == (void*)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    //���ù����ڴ�
    shared = (struct shared_use_st*)shm;
    int i = 0;
    while(i < msg_number)//�����ڴ���д����
    {
        //���ݻ�û�б���ȡ����ȴ����ݱ���ȡ,���������ڴ���д���ı�
        if (shared->written == 1)
        {
            sem_v(semid);
            sprintf(buffer, "%s%d\n", SENDER_MSG, i);
            strncpy(shared->text, buffer, strlen(buffer)+1);
            //д�����ݣ�����writtenʹ�����ڴ�οɶ�
            shared->written = 1;
            i++;
        }else {
            //�����ڴ���д������
            //printf("Enter some text: ");
            //fgets(buffer, BUFSIZ, stdin);
            sprintf(buffer, "%s%d\n", SENDER_MSG, i);
            strncpy(shared->text, buffer, strlen(buffer)+1);
            //д�����ݣ�����writtenʹ�����ڴ�οɶ�
            shared->written = 1;
            i++;
            sem_v(semid);
        }
        //printf("%s\n", buffer);
    }
    sprintf(buffer, "end");
    strncpy(shared->text, buffer, strlen(buffer)+1);
    //д�����ݣ�����writtenʹ�����ڴ�οɶ�
    shared->written = 1;
    //�ѹ����ڴ�ӵ�ǰ�����з���
    if(shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    sem_del(semid);
    exit(EXIT_SUCCESS);
}
