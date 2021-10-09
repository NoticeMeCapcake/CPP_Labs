#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int variant = 0;
    int cnt = 0, pid = 1;
    printf("%d -> parent = %d\n", getppid(), getpid());
    srand(time(NULL));
    while (1) {
        variant = rand() % 101 + 1;
        if (variant <= 25 && variant >= 1) {
            if (!(pid = fork())) cnt++;
        }
        else if (variant >= 26 && variant <= 45) {
            if ((pid = fork()) > 0) {
                if (!(pid = fork())) cnt++;
            }
            else cnt++;
        }
        else if (variant >= 46 && variant <= 75) {
            if ((pid = fork()) > 0) {
                if((pid = fork()) > 0) {
                    if (!(pid = fork())) cnt++;
                }
                else cnt++;
            }
            else cnt++;
        }
        else {
            wait(NULL);
            printf("%d -> %d: %d\n", getppid(), getpid(), cnt);
            exit(1);
        }
        if (pid) break;  // НАДО ЧТО-ТО ПРИДУМАТЬ
    }
    wait(NULL);
    return 0;
}
