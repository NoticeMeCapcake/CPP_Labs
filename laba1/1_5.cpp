#include <iostream>
#include <cstdarg>
#include <cctype>
#include <cstring>
#include <string>

using namespace std;

string great_sum(const int base, const int count, ...) {
    if (base < 2 or base > 36) return "...";
    va_list args;
    va_start(args, count);
    string res = va_arg(args, string);
    string left_oper, right_oper;
    for (int i = 0; i < count - 1; i++) {
        int mid_res = 0;
        left_oper = string(res);
        right_oper = va_arg(args, string);
        int len1 = left_oper.length();
        int len2 = right_oper.length();
        int len = (len1 >= len2) ? len1 : len2;
        for (int j = 0; j < abs(len1 - len2) + 1; j++) {
             if (len1 >= len2) {
                if (!j)
                    left_oper.insert(0, "0");
                right_oper.insert(0, "0");
             }
             else {
                 if (!j)
                    right_oper.insert(0, "0");
                 left_oper.insert(0, "0");
             }
        }

        for (int j = len; j >= 0; j--) {
            mid_res += toupper(left_oper[j]) - (isdigit(left_oper[j]) ? '0' : 'A' - 10) +
                    toupper(right_oper[j]) - (isdigit(right_oper[j]) ? '0' : 'A' - 10);
            left_oper[j] = mid_res % base + (mid_res % base < 10 ? '0' : 'A' - 10);
            mid_res /= base;
        }

        int zero_cnt = 0;
        for (int j = 0; j <= len; j++) {
            if (left_oper[j] != '0')
                break;
            zero_cnt++;
        }
        res.assign(left_oper, left_oper[0] != '0'? 0 : zero_cnt, len + (left_oper[0] != '0'? zero_cnt : 0));
    }

    va_end(args);
    return res;
}

int main() {
    cout << great_sum(11, 2, string("0A111"), string("9001")) << endl;
    return 0;
}

//mingw32-make
//-Wall -Wextra -Winit-self -Wuninitialized -Wshadow -Wwrite-strings -ftrapv
