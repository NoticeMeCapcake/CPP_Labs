#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct MsgType {
    long type;
    unsigned int shift;
    bool last;
    char msg[1000];
};

enum signal {
    SND = 5,
    RCV
};

void encrypt(char *str, int shift) {
    int real_shift = shift % 26;
    for (int i = 0; i < strlen(str); i++) {

        if (isalpha(str[i])) {
            int res = str[i] + real_shift;
            if (str[i] >= 'A' and str[i] <= 'Z') {
                if (res > 'Z') {
                    str[i] = 'A' + (res - 'Z');
                }
                else if (res < 'A') {
                    str[i] = 'Z' + (res - 'A');
                }
                else {
                    str[i] = res;
                }
            }
            else {
                if (res > 'z') {
                    str[i] = 'a' + (res - 'z');
                }
                else if (res < 'a') {
                    str[i] = 'z' + (res - 'a');
                }
                else {
                    str[i] = res;
                }
            }
        }
    }
}


int main() {
    MsgType queueMsg;
    struct msqid_ds QData;

    int qId = msgget(ftok("Server.cpp", 'q'), IPC_CREAT | 0666);

    if (qId < 0) {
        cout << "Error" << endl;
        exit(-1);
    }



    while(1) {
        FILE *res = fopen("SRes", "w");
        if (!res) {
            cout << "File error" << endl;
            exit(-5);
        }

        while (1) {
            if (msgrcv(qId, (void *) &queueMsg, sizeof(queueMsg), RCV, 0) < 0) {
                cout << "Server crushed" << endl;
                exit(-2);
            }

            encrypt(queueMsg.msg, queueMsg.shift);


            fprintf(res, "%s", queueMsg.msg);
            if (queueMsg.last) {
                break;
            }
        }

        fclose(res);

        queueMsg.type = SND;
        queueMsg.last = false;


        res = fopen("SRes", "r");
        if (!res) {
            cout << "File error" << endl;
            exit(-5);
        }
        while (1) {
            int cnt = fread(queueMsg.msg, sizeof(char), 999, res);
            queueMsg.msg[cnt] = '\0';
            if (cnt < 999) {
                if (feof(res)) {
                    queueMsg.shift = 0;
                    queueMsg.last = true;
                }
            }
            if (msgsnd(qId, (void *) &queueMsg, sizeof(queueMsg), 0) < 0) {
                 cout << "Can not send the answer" << endl;
                 msgctl(qId, IPC_RMID, &QData);
                 exit(-4);
             }
             if (queueMsg.last) {
                 cout << "broke" << endl;
                 break;
             }
        }
        fclose(res);
    }
    return 0;
}
