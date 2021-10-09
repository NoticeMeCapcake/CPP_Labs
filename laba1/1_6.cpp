#include <iostream>
#include <fstream>
#include <cstdarg>
#include <string>
#include <vector>
#include <cmath>
#include <functional>

using namespace std;

vector<unsigned long long> vals(26);

unsigned int bases[26];

void set_zero() {
    for (int i = 0; i < 26; i++) {
        // vals[i] = ;
        bases[i] = 0;
    }
}

bool check_system(const string &num, const int base) {
    if (base < 2 or base > 36)
        return false;
    for (size_t i = 0; i < num.length(); i++) {
        if (toupper(num[i]) - (isdigit(num[i]) ? '0' : 'A' - 11) > base)
            return false;
    }
    return true;
}

unsigned long long convert_in_decimal(const string &num, const int base) {  // перевод из любой в десятичку O(n)
    unsigned long long n = num.length(), accum = 0;
    for (unsigned long long i = 0; i < n; i++) {
        if (!isalnum(num[i])) throw -1;
        accum *= base;
        accum += isdigit(num[i])? num[i] - '0': toupper(num[i]) - 'A' + 10;
    }
    return accum;
}

string convert_in_base(unsigned long long num, const int base) {
    string accum;
    unsigned long long buf = 0;
    while(num) {
        buf = num % base;
        accum = string(1, char(buf < 10 ? buf + '0': buf + 'A' - 10)) + accum;
        cout << "accum = " << accum << endl;
        num /= base;
    }

    return accum;
}

unsigned long long disjunction(const unsigned long long &left, const unsigned long long &right) { //=
    return left | right;
}

unsigned long long conjunction(const unsigned long long &left, const unsigned long long &right) { //&
    return left & right;
}

unsigned long long equivalence(const unsigned long long &left, const unsigned long long &right) { //~
    return disjunction(conjunction(~left, ~right), conjunction(left, right));
}

unsigned long long implication(const unsigned long long &left, const unsigned long long &right) { //->
    return disjunction(~left, right);
}

unsigned long long reimplication(const unsigned long long &left, const unsigned long long &right) { //<-
    return disjunction(left, ~right);
}

unsigned long long coimplication(const unsigned long long &left, const unsigned long long &right) { //+>
    return conjunction(left, ~right);
}

unsigned long long sheffer(const unsigned long long &left, const unsigned long long &right) { //?
    return ~conjunction(left, right);
}

unsigned long long webb(const unsigned long long &left, const unsigned long long &right) { //!
    return ~disjunction(left, right);
}

unsigned long long xor_(const unsigned long long &left, const unsigned long long &right) { //<>
    return left ^ right;
}

// unsigned long long negation(const unsigned long long &num) {  /* \ */
//     return ~num;
// }

vector<function<unsigned long long (const unsigned long long &, const unsigned long long &)>> funcs;

int read(const unsigned int var, const unsigned int base) {
    if (base > 36 or base < 2) return -1; // сделать ошибку
    string inp;
    cin >> inp;
    for (size_t i = 0; i < inp.length(); i++) {
        if(!isalnum(inp[i])) return 0;
    }
    if (!check_system(inp, base)) return 0;
    vals[var] = convert_in_decimal(inp, base);
    bases[var] = base;
    return bases[var];
}

int write(const unsigned int var, const unsigned int base) {
    if (var > 35 or base < 2 or base > 36) return -1;
    cout << char('A' + var) << "(" << base << ") = " << (convert_in_base(vals[var], base)) << endl;
    return vals[var];
}

unsigned long long equals(const string &in) {
    if (!bases[in[0] - 'A']) return 0;
    if (in.length() < 4) {
        return 3; // not full instr
    }
    if (isalpha(in[3]) and !bases[in[3] - 'A']) {
        return 3; // uninitialized
    }
    if (!isalpha(in[3])) {
        return 3; // unrec symb
    }
    if (in.length() > 4) {
        return 4; // unrec symb
    }

    vals[in[0] - 'A'] = vals[in[3] - 'A'];
    return string::npos;
}

size_t find_com(const string &in, const vector<string> &coms) {
    size_t pos = string::npos;
    int ind = 0;
    for (auto com: coms) {
        if ((pos = in.find(com)) != string::npos) {
            if ((ind == 2 and pos != 1) or (ind != 2 and pos != 0)) {
                return string::npos;
            }
            return ind;
        }
        ind++;
    }
    return pos;
}

size_t find_oper(const string &in, const vector<string> &opers) {
    size_t pos = string::npos;
    size_t ind = 0;
    for (auto oper: opers) {
        if ((pos = in.find(oper)) != string::npos) {
            if ((ind == 9 and pos != 3) or (ind != 9 and pos != 4)) {
                return string::npos;
            }
            cout << "ind = " << ind << " pos = " << pos << endl;
            return ind;
        }
        ind++;
    }
    return string::npos;
}

unsigned long long select_operation(const string &in, const size_t ind) {
    if (!bases[in[0] - 'A']) return 0;
    if (ind == 9) {
        if (in.length() < 5) {
            return 4; // not full instr
        }
        if (!isalpha(in[4])) {
            return 4; // unrec symb
        }
        if (!bases[in[4] - 'A']) {
            return 4; // uninitialized
        }
        if (in.length() > 5) {
            return 5; // unrec symb
        }

        vals[in[0] - 'A'] = ~vals[in[4] - 'A'];
    }
    else if (ind < 5) {
        if (in.length() < 6) {
            return 5; // not full instr
        }
        if (!isalpha(in[3])) {
            return 3; // unrec symb
        }
        if (!bases[in[3] - 'A']) {
            return 3; // uninitialized
        }
        if (!isalpha(in[5])) {
            return 5; // unrec symb
        }
        if (!bases[in[5] - 'A']) {
            return 5; // uninitialized
        }
        if (in.length() > 6) {
            return 6; // unrec symb
        }
        vals[in[0] - 'A'] = funcs[ind](vals[in[3] - 'A'], vals[in[5] - 'A']);
    }
    else if (ind >= 5 and ind < 9) {
        if (in.length() < 7) {
            return 6; // not full instr
        }
        if (!isalpha(in[3])) {
            return 3; // unrec symb
        }
        if (!bases[in[3] - 'A']) {
            return 3; // uninitialized
        }
        if (!isalpha(in[6])) {
            return 5; // unrec symb
        }
        if (!bases[in[6] - 'A']) {
            return 5; // uninitialized
        }
        if (in.length() > 7) {
            return 7; // unrec symb
        }
        vals[in[0] - 'A'] = funcs[ind](vals[in[3] - 'A'], vals[in[6] - 'A']);
        // write(in[0] - 'A', 2)
    }
    return string::npos;
}

unsigned long long pre_write(const string &in) {
    if (in.length() < 10) {
        return in.length() - 1;
    }
    if (in[5] != '(') {
        return 5;
    }
    if (!isalpha(in[6])) {
        return 6; // unrec symb
    }
    if (!bases[in[6] - 'A']) {
        return 6; // uninitialized
    }
    if (in[7] != ',') {
        return 7;
    }
    size_t pos = 0;
    int base = 0;
    if (!isdigit(in[8])) {
        return 8; // unrec symb
    }

    base = stoi(in.substr(8), &pos, 10);
    if (pos > 2) {
        return 8;
    }

    if (in[pos + 8] != ')') {
        return pos + 8;
    }

    if (base > 36 or base < 2) {
        return 8;
    }

    if ((in.length() > 10 and pos == 1) or (in.length() > 11 and pos == 2)) {
        return pos + 8 + 1;
    }
    if (write(in[6] - 'A', base) < 0) return 8; // сделать обработку
    // cout << "in prewrite " << in << " base = " << base << " pos = " << pos << endl;
    return string::npos;
}


unsigned long long pre_read(const string &in) {
    if (in.length() < 9) {
        return in.length() - 1;
    }
    if (in[4] != '(') {
        return 4;
    }
    if (!isalpha(in[5])) {
        return 5; // unrec symb
    }
    if (in[6] != ',') {
        return 6;
    }
    if (!isdigit(in[7])) {
        return 7; // unrec symb
    }
    if (in[8] != ')') {
        return 8;
    }
    if (in.length() > 9) {
        return 9;
    }
    // try {
    int exit_code;
    while ((exit_code = read(in[5] - 'A', in[7] - '0')) <= 0) {
        if (exit_code < 0) return 7; // сделать обработку
    }
    // }
    // catch(all) {
        // return 6; // переделать
    // }
    return string::npos;
}


unsigned long long interpretate(const string &in) {
    size_t pos;
    vector<string> coms;
    coms.push_back("READ");
    coms.push_back("WRITE");
    coms.push_back(":=");
    vector<string> opers;
    opers.push_back("+");
    opers.push_back("&");
    opers.push_back("?");
    opers.push_back("!");
    opers.push_back("~");
    opers.push_back("->");
    opers.push_back("<-");
    opers.push_back("<>");
    opers.push_back("+>");
    opers.push_back("\\");

    cout << in << endl;

    if(!isalpha(in[0])) return 0;

    if ((pos = find_com(in, coms)) == string::npos) {
        cout << "here1\n";
        return 1;
    }

    if (pos == 0) {
        cout << "here2\n";
        if ((pos = pre_read(in)) != string::npos) {
            return pos;
        }
    }
    else if (pos == 1) {
        if ((pos = pre_write(in)) != string::npos) {
            cout << "here3 -- " << pos << endl; // 8
            return pos;
        }
    }
    else if (pos == 2) {
        cout << "here4\n";
        if ((pos = find_oper(in, opers)) == string::npos) {
            cout << "after find oper = " << pos << endl;
            if ((pos = equals(in)) != string::npos) {
                cout << "pos after eq = " << pos << endl;
                return pos;
            }
        }
        else {
            if ((pos = select_operation(in, pos)) != string::npos) {
                return pos;
            }
        }
    }
    // cout << "before return" << endl;
    return string::npos;
}


// TODO: сделать обработчики ошибок И добавать ошибку UNDEFINED BEHAVIOR

int main(int argc, char const *argv[]) {
    set_zero();
    FILE *fin = fopen("inp", "r");
    char c;
    int incomment = 0;
    string command;
    size_t pos;
    long long line_cnt = 1;
    funcs.push_back(disjunction);
    funcs.push_back(conjunction);
    funcs.push_back(sheffer);
    funcs.push_back(webb);
    funcs.push_back(equivalence);
    funcs.push_back(implication);
    funcs.push_back(reimplication);
    funcs.push_back(xor_);
    funcs.push_back(coimplication);

    while (!feof(fin)) {  // цикл с обработкой конца файла

        c = fgetc(fin);
        if (c == '%' && !incomment) {
            while ((c = fgetc(fin)) != '\n' && c != EOF);
        }

        else if (c == '{') {
            incomment++;
        }

        else if (c == '}') {
            if (!incomment) return -1;
            incomment--;
        }

        else if (c == EOF && incomment) return -1; //... обработка
        else if (!incomment) {
            if (isspace(c)) {
                if (c == '\n') {
                    line_cnt++;
                }
                continue;
            }
            if (c == ';') {
                pos = string::npos;
                if ((pos = interpretate(command)) != string::npos) {
                    cout << "ERROR in line " << line_cnt << " position " << pos << endl;
                }
                command.clear();
            }
            else {
                command += toupper(c);
            }
        }
    }
    if (command.length()) return -1;
    cout << funcs[0](0, 1) << endl;
    return 0;
}
