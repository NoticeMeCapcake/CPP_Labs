#include <iostream>
#include <fstream>
#include <cstdarg>
#include <string>
#include <vector>
using namespace std;


int find_substr(const string &source, const string &substr) {
    int sub_len = substr.length();
    int delta_len = source.length() - sub_len;
    for (int i = 0; i <= delta_len; i++) {
		for (int j = 0;;j++) {
			if (j == sub_len) return i; // out of range of substr
			if(source[i + j] != substr[j]) break; // symbolic comparison
		}
	}
	return -1;
}


vector<string> find(int find_(const string &source, const string &substr),
                            const string &substr, int count, ...) {

    va_list args;
    va_start(args, count);
    vector<string> file_list;
    int sub_len = substr.length();
    char symb[sub_len + 1];
    symb[sub_len] = 0;
    for (int i = 0; i < count; i++) {
        string file_name = va_arg(args, string);
        ifstream fin(file_name);
        if (!fin.is_open()) continue;
        while (fin) {
            fin.read(symb, sub_len);
            if(fin.eof()) break;
            cout << symb << endl;
            if (find_(string(symb), substr) >= 0) {
                file_list.push_back(file_name);
                break;
            }
            fin.seekg(1 - sub_len, ios_base::cur);
        }
    }
    va_end(args);
    return file_list;
}


int main() {
    vector<string> list_of_matched_files = find(find_substr, "abc\n", 3, string("in1"), string("in3"), string("in2"));
    for (int i = 0; i < list_of_matched_files.size(); i++)
        cout << list_of_matched_files[i] << endl;
    return 0;
}
