#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int main() {
    DIR *direct = NULL;
    struct dirent *walker;
    char name[200];
    while (!direct) {
        printf("Enter directory path/name \n>");
        scanf("%199s", name);
        direct = opendir(name);
        if (!direct) {
            printf("%s: %s\n", "Can not open", name);
        }
    }
    printf("%s\n", "Task one:");
    while (walker = readdir(direct)) {
        printf("Name of the read file: %s\n", walker->d_name);
    }
    direct = opendir(name);
    printf("%s\n", "Task two:");
    while (walker = readdir(direct)) {
        if (walker->d_type != 4)
            printf("Name of the read file: %s\n", walker->d_name);
    }
    return 0;
}
