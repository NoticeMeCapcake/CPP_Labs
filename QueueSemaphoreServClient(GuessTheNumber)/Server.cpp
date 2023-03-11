#include <stdio.h>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>


enum signal {
    RCV = 5,
    SND
};


struct MsgType {
    long type;
    unsigned int shift;
    bool last;
    char msg[1000];
};


using namespace std;


int main()
{
    int sock, listen_soc;
    struct sockaddr_in addr;
    MsgType msg;
    int bytes_read, total = 0;

    listen_soc = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_soc < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listen_soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listen_soc, 1);



    MsgType queueMsg;


    int qId = msgget(ftok("Server.cpp", 'q'), 0);
    if (qId < 0) {
        cout << "Error, can\'t create a message queue" << endl;
        close(sock);
        exit(-1);
    }




    struct sembuf operations[2];
    MsgType *sh_mem;
    int semid, shmid;

    semid = semget(ftok("Server.cpp", 's'), 2, 0666);
    if (semid == -1) {
        printf("semget() failed\n");
        return -1;
    }

    shmid = shmget(ftok("Server.cpp", 'm'), sizeof(MsgType), 0666);
    if (shmid == -1) {
        printf("shmget() failed\n");
        return -1;
    }

    sh_mem = (MsgType *) shmat(shmid, NULL, 0);

    if (sh_mem == nullptr) {
        printf("shmat() failed\n");
        close(sock);
        return -1;
    }

    operations[0].sem_num = 0;
    operations[0].sem_flg = 0;
    operations[1].sem_num = 1;
    operations[1].sem_flg = 0;

    while(1)
    {
        sock = accept(listen_soc, (struct sockaddr *)NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            close(sock);
            if (shmdt(sh_mem) == -1) {
                printf("shmdt() failed\n");
                return -1;
            }
            exit(-3);
        }

        if (!fork()) {
            while(1) {
                bytes_read = recv(sock, &msg, sizeof(msg), 0);
                if(bytes_read <= 0) {
                    break;
                }
    /////////////////////////////////////// QUEUE
                if (msg.type == 0) {
                    queueMsg.type = SND;
                    queueMsg.shift = msg.shift;
                    queueMsg.last = msg.last;
                    strcpy(queueMsg.msg, msg.msg);
                    if (msgsnd(qId, (void *) &queueMsg, sizeof(queueMsg), 0) != 0) {
                        cout << "Error, can not send your message" << endl;
                        exit(-1);
                    }

                    if (!msg.last)
                        continue;
                    queueMsg.last = false;
                    while (1) {
                        if (msgrcv(qId, (void *) &queueMsg, sizeof(queueMsg), RCV, 0) < 0) {
                            cout << "Server crushed" << endl;
                            exit(-2);
                        }
                        if (send(sock, &queueMsg, sizeof(queueMsg), 0) < 0) {
                            cout << "Server crushed" << endl;
                            exit(-2);
                        }
                        if (queueMsg.last) {
                            return(0);
                        }
                    }
                }
    ///////////////////////////////SEMS
                else {
                    operations[0].sem_op = 0;
                    operations[1].sem_op = -1;
                    if (semop(semid, operations, 2) == -1) {
                        printf("semop() failed\n");
                        return -1;
                    }

                    strcpy(sh_mem->msg, msg.msg);
                    sh_mem->type = msg.type;
                    sh_mem->shift = msg.shift;


                    operations[0].sem_op = 1;
                    operations[1].sem_op = 0;

                    if (semop(semid, operations, 2) == -1) {
                        printf("semop() failed\n");
                        return -1;
                    }


                    operations[0].sem_op = -1;
                    operations[1].sem_op = -1;
                    if (semop(semid, operations, 2) == -1) {
                        printf("semop() failed111\n");
                        return -1;
                    }

                    strcpy(msg.msg, sh_mem->msg);
                    msg.type = sh_mem->type;

                    if (send(sock, &msg, sizeof(msg), 0) < 0) {
                        cout << "Server crushed" << endl;
                        exit(-2);
                    }

                    operations[0].sem_op = 0;
                    operations[1].sem_op = 1;
                    if (semop(semid, operations, 2) == -1) {
                        printf("semop() failed\n");

                        return -1;
                    }
                    if (string(msg.msg) == "win") {
                        return 0;
                    }
                }
            }
        }
        else {
            wait(0);
            close(sock);
        }
    }

    return 0;
}
