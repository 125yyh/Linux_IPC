#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmdata.h"
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

int main()
{
    int running = 1;//�����Ƿ�������еı�־
    void *shm = NULL;//����Ĺ����ڴ��ԭʼ�׵�ַ
    struct shared_use_st *shared;//ָ��shm

    int semid;
    semid = sem_init();
    //���������ڴ�
    int shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if(shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
    shm = shmat(shmid, 0, 0);
    if(shm == (void*)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    shared = (struct shared_use_st*)shm;
    shared->written = 0;
    while(running)//��ȡ�����ڴ��е�����
    {
        //û�н��������ڴ涨���������ݿɶ�ȡ
        if(shared->written == 1)
        {
            //sleep(rand() % 3);
            //��ȡ�����ݣ�����writtenʹ�����ڴ�ο�д
            //printf("You wrote: %s", shared->text);
            shared->written = 0;
            //������end���˳�ѭ��������
            if(strncmp(shared->text, "end", 3) == 0)
            {
                running = 0;
                printf("get end comand\n");
            }
            sem_v(semid);
        }
        else//������������д���ݣ����ܶ�ȡ����
        {
            sem_p(semid);
            //printf("You wrote: %s", shared->text);
            shared->written = 0;
            if(strncmp(shared->text, "end", 3) == 0)
            {
                running = 0;
                printf("get end comand\n");
            }

        }
    }
    //�ѹ����ڴ�ӵ�ǰ�����з���
    if(shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    //ɾ�������ڴ�
    if(shmctl(shmid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
