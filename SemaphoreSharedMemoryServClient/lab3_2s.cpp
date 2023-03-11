#include <stdio.h>
#include <cstring>
#include <string>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define SEMKEYPATH "./lab3_2s.cpp"
#define SEMKEYID 's'
#define SHMKEYPATH "./lab3_2c.cpp"
#define SHMKEYID 'm'
#define NUMSEMS 2

using namespace std;

struct Msg {
    char msg[200];
};

double ex(int op, double left, double right) {
    switch(op) {
        case 0:
        return left + right;
        case 1:
        return left - right;
        case 2:
        return left * right;
        case 3:
        if (!right) {
            throw -1;
        }
        return left / right;
    }
    return 0;
}

void calculate(char *expr) {
    string full_expr(expr);
    char opers[4][2] = {"+", "-", "*", "/"};
    unsigned long long pos = full_expr.find(" ");
    string first_operand, second_operand, operation;
    first_operand.assign(full_expr, 0, pos);
    operation.assign(full_expr, pos+1, 1);
    second_operand.assign(full_expr, pos+3);

    for (int i = 0; i < 4; i++) {
        if (operation == opers[i]) {
            try {
                strcpy(expr, to_string(ex(i, stod(first_operand), stod(second_operand))).c_str());
            }
            catch(int) {
                strcpy(expr, "zero");
            }
            return;
        }
    }
    return;
}

int main() {
    int semid, shmid;
    key_t semkey, shmkey;
    Msg *shm_address;
    struct sembuf operations[2];
    struct shmid_ds shmid_struct;
    short sarray[NUMSEMS];

    semkey = ftok(SEMKEYPATH,SEMKEYID);
    if (semkey == (key_t) -1) {
        printf("main: ftok() for sem failed\n");
        return -1;
    }

    shmkey = ftok(SHMKEYPATH,SHMKEYID);

    if (shmkey == (key_t) -1) {
        printf("main: ftok() for shm failed\n");
        return -1;
    }

    semid = semget( semkey, NUMSEMS, 0666 | IPC_CREAT );
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

    shmid = shmget(shmkey, sizeof(Msg), 0666 | IPC_CREAT);
    if (shmid == -1) {
        printf("shmget() failed\n");
        return -1;
    }

    shm_address = (Msg *) shmat(shmid, NULL, 0);
    if (shm_address == nullptr)
    {
        printf("mshmat() failed\n");
        return -1;
    }

    operations[0].sem_num = 0;
    operations[0].sem_flg = 0;
    operations[1].sem_num = 1;
    operations[1].sem_flg = 0;
    while(1) {

        operations[0].sem_op = -1;
        operations[1].sem_op = 0;

        printf("Waiting for client\'s request\n");
        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }
        
        if (strlen(shm_address->msg)) {
            calculate(shm_address->msg);
        }


        operations[0].sem_op = 1;
        operations[1].sem_op = 1;
        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }
    }

}
