#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define SEMKEYPATH "./lab3_2s.cpp" /* Путь, используемый в ftok при генерации ключа для semget */
#define SEMKEYID 's' /* Id идентификатор проекта в ftok для semget ключа key */
#define SHMKEYPATH "./lab3_2c.cpp" /* Путь, используемый в ftok при генерации ключа для shmget */
#define SHMKEYID 'm' /* Id идентификатор проекта в ftok для shmget ключа key */
#define NUMSEMS 2 /* кол-во семафоров в наборе */

using namespace std;

struct Msg {
    char msg[200];
};

#define SIZEOFSHMSEG sizeof(Msg) /* размер сегмента разделяемой памяти */

int main() {
    struct sembuf operations[2];
    Msg *shm_address;
    int semid, shmid, rc;
    key_t semkey, shmkey;
    string input, output;

    semkey = ftok(SEMKEYPATH, SEMKEYID);
    if ( semkey == (key_t)-1 ) {
        printf("ftok() for sem failed\n");
        return -1;
    }

    shmkey = ftok(SHMKEYPATH, SHMKEYID);
    if ( shmkey == (key_t)-1 ) {
        printf("ftok() for shm failed\n");
        return -1;
    }

    semid = semget(semkey, NUMSEMS, 0666);
    if (semid == -1) {
        printf("semget() failed\n");
        return -1;
    }

    shmid = shmget(shmkey, SIZEOFSHMSEG, 0666);
    if (shmid == -1) {
        printf("shmget() failed\n");
        return -1;
    }

    shm_address = (Msg *) shmat(shmid, NULL, 0);

    if (shm_address == nullptr) {
        printf("shmat() failed\n");
        return -1;
    }

    operations[0].sem_num = 0;
    operations[0].sem_flg = 0;
    operations[1].sem_num = 1;
    operations[1].sem_flg = 0;
    while (1) {

        printf("Enter math expression\n>>");
        getline(cin, input);

        if (input.length() == 0) {
            if (shmdt(shm_address) == -1) {
                printf("shmdt() failed\n");
                return -1;
            }
            printf("Shut down\n");
            return -1;
        }

        operations[0].sem_op = 0;
        operations[1].sem_op = -1;
        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }
        strcpy(shm_address->msg, input.c_str());


        operations[0].sem_op = 1;
        operations[1].sem_op = 0;

        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }

        operations[0].sem_op = -1;
        operations[1].sem_op = -1;
        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }

        if (string(shm_address->msg) == "zero") {
            printf("Answer: division by zero\n");
        }
        printf("Answer: %s\n", shm_address->msg);

        operations[0].sem_op = 0;
        operations[1].sem_op = 1;
        if (semop(semid, operations, 2) == -1) {
            printf("semop() failed\n");
            return -1;
        }
    }
}
