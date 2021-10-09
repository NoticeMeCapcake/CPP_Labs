#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *arifm_exp = NULL, *ptrEnd;
    double f_oper = 0, s_oper = 0, result = 0;
    char operations_list[] = "+-*/";
    int oper_index = -1;
    FILE *fout;
    if (argc != 3) {
        return -3;
    }

    fout = fopen(argv[2], "a");
    if (fout == NULL) return -5;

    arifm_exp = argv[1];

    f_oper = strtod(arifm_exp, &ptrEnd);

    for(int i = 0; i < strlen(operations_list); i++) {
        if (*ptrEnd == operations_list[i])
            oper_index = i;
    }
    if (oper_index == -1 || (!isdigit(*(++ptrEnd)) && *ptrEnd != '-')) {
        fprintf(fout, "Error\n");
        fclose(fout);
        return -1;
    }


    s_oper = strtod(ptrEnd, &ptrEnd);
    if (*ptrEnd != '\0') {
        fprintf(fout, "Error\n");
        fclose(fout);
        return -1;
    }


    switch (oper_index) {
        case 0:
        result = f_oper + s_oper;
        break;

        case 1:
        result = f_oper - s_oper;
        break;

        case 2:
        result = f_oper * s_oper;
        break;

        case 3:
        if (s_oper == 0) {
        fprintf(fout, "division by zero\n");
        fclose(fout);
        return -7;
        }
        result = f_oper / s_oper;
        break;
    }

    fprintf(fout, "%.3lf\n", result);

    fclose(fout);
    return 0;
}
