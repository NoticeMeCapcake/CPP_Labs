#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char name[] = "lab1a";
    FILE *fin, *fout;
    char line[100];
    char *new_argv[4];
    int i, non_space_ind;
    if (argc != 3)
        return -3;

    new_argv[0] = malloc(sizeof(name));
    strcpy(new_argv[0], name);

    new_argv[2] = malloc(strlen(argv[2]) + 1);
    strcpy(new_argv[2], argv[2]);

    new_argv[3] = 0;

    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        printf("%s\n", "Can not open input file");
        return -1;
    }
    while (fgets(line, sizeof(line) - 1, fin)) {
        fout = fopen(argv[2], "a");
        if (fout == NULL) return -4;
        non_space_ind = 0;
        for(i = 0; i < strlen(line); i++) {
            if (!isspace(line[i])) {
                line[non_space_ind++] = line[i];
            }
        }
        line[non_space_ind] = '\0';


        new_argv[1] = malloc(strlen(line) + 1);
        strcpy(new_argv[1], line);

        fprintf(fout, "%s = ", line);
        fclose(fout);
        switch(fork()) {
            case -1:
            // fprintf(fout, "%s", "can not calculate");
            break;

            case 0:

            // fclose(fout);
            if (execv(name, new_argv) < 0) {
                // fout = fopen(argv[2], "a");
                // fprintf(fout, "%s", "can not calculate");
            }
            default:
            wait(0);
        }
        free(new_argv[1]);
    }
    fclose(fin);
    return 0;
}
