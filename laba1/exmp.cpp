#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;



int main(int argc, char const *argv[]) {
    ifstream fin("in");
    string buf;

    if(!fin.is_open()) {
        cout << "Error. Input file can not be opened." << endl;
        exit(-1);
    }

    while(1) {
        fin >> buf;
        if(!buf.length()) {
            return 0;
        }
        cout << buf << endl;
        buf.clear();
    }

    // ofstream fout("out");
    // if(!fin.is_open()) {
    //     cout << "Error. Output file can not be opened." << endl;
    //     exit(-1);
    // }



    return 0;
}
