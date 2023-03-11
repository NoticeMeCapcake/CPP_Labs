#include <stdio.h>
#include <cstring>
#include <string>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <ctime>
#include <cstdlib>
#include <string>

struct MsgType {
    long type;
    unsigned int shift;
    bool last;
    char msg[1000];
};

int compare(long guess_num, unsigned int guessing) {
    if (guessing > guess_num) {
        return 1;
    }
    else if (guessing < guess_num) {
        return -1;
    }
    else {
        return 0;
    }
}

using namespace std;

int main() {
    int semid, shmid;
    key_t semkey, shmkey;
    MsgType *sh_mem;
    struct sembuf operations[2];
    struct shmid_ds shmid_struct;
    short sarray[2];

    srand(time(0));

    semkey = ftok("Server.cpp", 's');
    if (semkey == (key_t) -1) {
        printf("main: ftok() for sem failed\n");
        return -1;
    }

    shmkey = ftok("Server.cpp", 'm');

    if (shmkey == (key_t) -1) {
        printf("main: ftok() for shm failed\n");
        return -1;
    }

    semid = semget( semkey, 2, 0666 | IPC_CREAT );
    if (semid == -1) {
        printf("main: semget() failed\n");
        return -1;
    }

    sarray[0] = 0;
    sarray[1] = 1;
    if(semctl(semid, 1, SETALL, sarray) == -1) {
        printf("semctl() initialization failed\n");
        return -1;
    }

    shmid = shmget(shmkey, sizeof(MsgType), 0666 | IPC_CREAT);
    if (shmid == -1) {
        printf("shmget() failed\n");
        return -1;
    }

    sh_mem = (MsgType *) shmat(shmid, NULL, 0);
    if (sh_mem == nullptr)
    {
        printf("shmat() failed\n");
        return -1;
    }

    operations[0].sem_num = 0;
    operations[0].sem_flg = 0;
    operations[1].sem_num = 1;
    operations[1].sem_flg = 0;
    while (1) { 
        operations[0].sem_op = -1;
        operations[1].sem_op = 0;

        printf("Waiting for client\'s request\n");
        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }
        if (string(sh_mem->msg) == "create") {
            sh_mem->type = rand() % 10000 + 1;
        }
        else {
            int res = compare(sh_mem->type, sh_mem->shift);

            if (res < 0) {
                strcpy(sh_mem->msg, "low");
            }
            else if (res > 0) {
                strcpy(sh_mem->msg, "high");
            }
            else {
                strcpy(sh_mem->msg, "win");
            }
        }

        operations[0].sem_op = 1;
        operations[1].sem_op = 1;
        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }

    }
}
