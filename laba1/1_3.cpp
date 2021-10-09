#include <iostream>
#include <cstdarg>
// #include <string>
#include <vector>

using namespace std;

double calculate(const double x, const int degree, ...) { // значения коэффициентов подаются с большей степени
    va_list args;
    va_start(args, degree);
    int res = 0;
    for (int i = 0; i <= degree; i++) {
        res *= x;
        res += va_arg(args, double);
    }
    return res;
}

int main() {
    cout << calculate(2.0, 3, -1.0, 2.0, 2.0, -7.0) << endl;
    return 0;
}
