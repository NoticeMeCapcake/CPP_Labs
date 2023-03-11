#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct SndMsg {
    long type;
    double coef1;
    double coef2;
    double coef3;
    double coef4;
};

struct RcvMsg {
    long type;
    short rootType;
    double froot;
    double sroot;
    double troot;
};


int main() {
    SndMsg sndMsg;
    RcvMsg rcvMsg;

// ftok("lab3_1c.cpp", 'c')
// ftok("lab3_1s.cpp", 's')
    int outqId = msgget(ftok("lab3_1c.cpp", 'c'), 0);
    int inqId = msgget(ftok("lab3_1s.cpp", 's'), 0);

    if ((outqId < 0) or (inqId < 0)) {
        cout << "Error" << endl;
        exit(-1);
    }

    sndMsg.type = getpid();
    cout << "Enter the first coefficient\n>>";
    cin >> sndMsg.coef1;
    cout << "Enter the second coefficient\n>>";
    cin >> sndMsg.coef2;
    cout << "Enter the third coefficient\n>>";
    cin >> sndMsg.coef3;
    cout << "Enter the fourth coefficient\n>>";
    cin >> sndMsg.coef4;

    if (sndMsg.coef1 == 0) {
        sndMsg.type = 1;
    }
    cout << "Sending message to the server..." << endl;

    if (msgsnd(outqId, (void *) &sndMsg, sizeof(sndMsg), 0) != 0) {
        cout << "Error, can not send your message" << endl;
        exit(-1);
    }
    else {
        cout << "Message sent successfully!" << endl;
        if (sndMsg.coef1 == 0) {
            cout << "Shutting down the server" << endl;
            exit(1);
        }
        // exit(-1);
    }

    if (msgrcv(inqId, (void *) &rcvMsg, sizeof(rcvMsg), sndMsg.type, 0) < 0) {
        cout << "Can not reciev answer" << endl;
        exit(1);
    }
    else {
        cout << "Recieved answer" << endl;
        switch (rcvMsg.rootType) {
            case 10:
            cout << "Error you had been sent the wrong value of the fist coefficient" << endl;
            break;
            case 1:
            cout << "X_1 = " << rcvMsg.froot << "; X_2 = " <<
                    rcvMsg.sroot << "; X_3 = " << rcvMsg.troot << endl;
            break;
            case 0:
            cout << "X_1 = " << rcvMsg.froot << "; X_2 = X_3 = " <<
                    rcvMsg.sroot << endl;
            break;
            case -1:
            cout << "X_1 = " << rcvMsg.froot << "; X_2 = " <<
                    rcvMsg.sroot << " + i*" << rcvMsg.troot << "; X_3 = " <<
                    rcvMsg.sroot << " - i*" << rcvMsg.troot << endl;
            break;

        }
    }

    exit(0);
}
