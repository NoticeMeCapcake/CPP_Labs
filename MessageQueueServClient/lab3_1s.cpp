#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <math.h>
#define M_2PI (2.*M_PI)

using namespace std;

struct RcvMsg {
    long type;
    double coef1;
    double coef2;
    double coef3;
    double coef4;
};

struct SndMsg {
    long type;
    short rootType;
    double froot;
    double sroot;
    double troot;
};

int sign(float a) {
    if (a < 0) {
        return -1;
    }
    else {
        return 1;
    }
}

void solveCubic(SndMsg *x, double a, double b, double c) {
  double q,r,r2,q3;
  q = (a*a - 3.*b) / 9.;
  r = (a * (2.*a*a - 9.*b) + 27.*c) / 54.;
  r2 = r*r;
  q3 = q*q*q;

  if (r2 < q3) {
    double t = acos(r / sqrt(q3));

    a /= 3.;
    q = -2. * sqrt(q);

    x->froot = q * cos(t / 3.) - a;
    x->sroot = q * cos((t + M_2PI) / 3.) - a;
    x->troot = q * cos((t - M_2PI) / 3.) - a;

    x->rootType = 1;
  }
  else {
    double aa,bb;

    aa = -sign(r) * pow(fabs(r) + sqrt(r2 - q3), 1./3.);

    if (aa != 0.)
        bb = q / aa;
    else
        bb = 0.;

    a /= 3.;
    q = aa + bb;
    r = aa - bb;

    x->froot =q - a;
    x->sroot = (-0.5) * q - a;
    x->troot = (sqrt(3.) * 0.5) * fabs(r);

    if(x->troot == 0)
        x->rootType = 0;
    else
        x->rootType = -1;
  }
}


int main() {
    SndMsg sndMsg;
    RcvMsg rcvMsg;
    struct msqid_ds QData;

    int inqId = msgget(ftok("lab3_1c.cpp", 'c'), IPC_CREAT | 0666);
    int outqId = msgget(ftok("lab3_1s.cpp", 's'), IPC_CREAT | 0666);

    if ((outqId < 0) or (inqId < 0)) {
        cout << "Error" << endl;
        exit(-1);
    }

    while(1) {
        if (msgrcv(inqId, (void *) &rcvMsg, sizeof(rcvMsg), 0, 0) < 0) {
            cout << "Server crushed" << endl;
            exit(-2);
        }
        std::cout << "MSG cought" << endl;
        if (rcvMsg.type == 1) {
            cout << "Shutting down" << endl;
            msgctl(inqId, IPC_RMID, &QData);
            msgctl(outqId, IPC_RMID, &QData);
            exit(1);
        }
        sndMsg.type = rcvMsg.type;
        if (rcvMsg.coef1 == 0) {
            sndMsg.rootType = 10;
        }
        else {
            solveCubic(&sndMsg, rcvMsg.coef2/rcvMsg.coef1, rcvMsg.coef3/rcvMsg.coef1,
                    rcvMsg.coef4/rcvMsg.coef1);
         }

         if (msgsnd(outqId, (void *) &sndMsg, sizeof(SndMsg), 0) < 0)
         {
             cout << "Can not send the answer" << endl;
             msgctl(inqId, IPC_RMID, &QData);
             msgctl(outqId, IPC_RMID, &QData);
             exit(-4);
         }
         cout << "Messege had been sent" << endl;
    }

    return 0;
}
