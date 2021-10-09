#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int min_base(const string &num) {
    int base = 0, symbol = 0;
    for (int i = 0; i < num.length(); i++) {
        symbol = 1 + (isdigit(num[i]) ? num[i] - '0' : (isalpha(num[i]) ? toupper(num[i]) - 'A' + 10 : -2));
        if (symbol == -1) return -1; // Ошибка (левый символ)
        if (symbol > base) base = symbol;
    }
    return base;
}

int convert_in_decimal(const string &num, const int base) {  // перевод из любой в десятичку O(n)
    int n = num.length(), accum = 0;
    for (int i = 0; i < n; i++) {
        accum *= base;
        accum += isdigit(num[i])? num[i] - '0': toupper(num[i]) - 'A' + 10;
    }
    return accum;
}

int main(int argc, char const *argv[]) {

    string num;


    if (argc != 3) {
        cout << "Error. You should write names of files as parameters." << endl;
        exit(-1);
    }
    ifstream fin(argv[1]);

    if(!fin.is_open()) {
        cout << "Error. Input file can not be opened." << endl;
        exit(-1);
    }

    ofstream fout(argv[2]);
    if(!fin.is_open()) {
        cout << "Error. Output file can not be opened." << endl;
        exit(-1);
    }

    while(1) {
        fin >> num;
        if(!num.length()) {
            return 0;
        }
        int base = min_base(num);
        if (base < 0) {
            fout << num << " - not a number" << endl;
            continue;
        }

        int decimal_num = convert_in_decimal(num, base);

        fout << num << " " << base << " " << decimal_num << endl;

        num.clear();
    }



    return 0;
}
