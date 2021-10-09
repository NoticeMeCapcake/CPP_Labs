#include <iostream>
#include <cctype>
#include <cstring>

using namespace std;

int convert_in_decimal(string num, const int base) {  // перевод из любой в десятичку O(n)
    int n = num.length(), accum = 0;
    for (int i = 0; i < n; i++) {
        accum *= base + (isdigit(num[i])? num[i] - '0': toupper(num[i]) - 'A' + 10);
    }
    return accum;
}

string convert_in_base(int num, const int base) {  //log_k(X)
    string accum;
    int buf = 0;
    while(num) {
        buf = num % base;
        accum = string(1, char(buf < 10 ? buf + '0': buf + 'A' - 10)) + accum;
        num /= base;
    }

    return accum;
}

char* concut(int count, ...) {
    va_list args;
    char *res = (char*) malloc(sizeof(char));
    char *arg;
    *res = 0;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        arg = va_arg(args, char*);
        res = (char*) realloc(res, (strlen(res) + strlen(arg) + 1) * sizeof(char));
        strcpy(res + strlen(res), arg);
    }
    va_end(args);
    return res;
}

int inter() {
    char regs[26];
    #define REG(x) regs[toupper(#x[0]) - 'A']  // либо toupper(x) - 'A'
}

// O(n) * O(log_2(k)) (n - длина имени переменной, k - длина массива) для 6 задачи
{
    FILE *fin = fopen("...", "r");
    char c;
    int incomment = 0;
    while (!feof(fin)) {  // цикл с обработкой конца файла
        c = fgetc(fin);
        if (c == '#' && !incomment) {
            while ((c = fgetc(fin)) != '\n' && c != EOF);
        }

        else if (c == '[') {
            incomment++;
        }

        else if (c == ']') {
            if (!incomment) {ERR}
            incomment--;
        }

        else if (c == EOF && incomment){ERR} //... обработка
        else if (!incomment) {

        }
    }

    // OR

    while  ((c = fgetc(fin)) != EOF) {}  // цикл без обработки конца файла
}



{
    READ(F, 16);
    READ(E, 23);
    D = E -> F; // ПРОБЕЛЫ ИГНОРИМ
    
}



int main() {
    cout << convert_in_base(215, 7) << endl;
    return 0;
}
