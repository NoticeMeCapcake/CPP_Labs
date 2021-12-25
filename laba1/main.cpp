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

-----------------------------------------------------------

тесты на простоту ферма и соловея-штрассена и есть

1. p, q : p!=q; p,q - простые , bitlen(p) = bitlen(q)

создаём массив  чаров и пихаем в него биты рандомного числа
старший бит и младший ставим в 1

2. p*q = N (Карацуба, Фурье)


3. phi(N) = phi(p*q) = phi(p)*phi(q) = (p-1)(q-1)  phi() -- ф-я эйлера
phi(p) = (p-1)

0--1--|---|-------------|----->x
      q   p             pq

pq - (p) - (q) + 1 = pq  -q - p + 1

p(q-1) - = (q - 1)(p + 1)

полняя система вычетов - все отатки по mod k
приведённая  система вычетов
ПСВ \i:(i, k) != 1

Лемма
(m, m') = 1
тогда
a :0 ---> m-1
a':0 ---? m'-1
am' + a'm: 0 ---> m*m' - 1*mm' - m*m'
(a1, a1') != a2 * a2'
a1*m' + a1'*m = a2*m' +a2'*m (mod m*m') | \mod m
m'(a1-a2) = 0 (mod m1'*m1)

phi(m*n) = pfi(n) / phi(m,n)



am' + a'm (mod m*m')


4. e; нод(е, фи(N)) = 1
