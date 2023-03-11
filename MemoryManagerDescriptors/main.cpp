#include <iostream>
#include <fstream>
#include "w_malloc.h"
#include <vector>
#include <regex>
#include <map>

//using namespace std;




class Interpreter {
    struct Variable {
        int refCount = 0;
        Obj *child = nullptr;
    };

    map<string, Variable> variables;

public:
    void interpret(vector<string> &strings);  // добавить функцию выделение лексем (разбить на подуровни)
    void update(const string &source, const string &target);
    void read_files(const vector<string> &files);
    void clear();
};


void Interpreter::update(const string &source, const string &target) {  //var1(target) = var2(source)

    variables[source].child->refCount++;
    if (variables[target].child != nullptr) {
        if (!(--variables[target].child->refCount)) {
            w_free(variables[target].child->ptr);
            delete(variables[target].child);
        }
    }
    variables[target].child = variables[source].child;
}


void Interpreter::interpret(vector<string> &strings) {

    regex reg_dealloc("dealloc\\(([a-zA-Z]\\w*)\\)");
    regex reg_alloc("([a-zA-Z]\\w*)=alloc\\(([1-9]\\d*)\\)");
    regex reg_assign("([a-zA-Z]\\w*)=([a-zA-Z]\\w*)");

    int line_cnt = 0;

    for (auto line : strings) {
        line_cnt++;
        smatch sub_line;
        if (regex_search(line, sub_line, reg_dealloc)) {
            cout << line_cnt << ": " << "Dealloc" << endl;

            if (variables.find(sub_line[1]) != variables.end()) {

                Variable &var_to_del = variables[sub_line[1]];
                if (!(--var_to_del.child->refCount)) {
                    w_free(var_to_del.child->ptr);
                    delete(var_to_del.child);
                }
                variables.erase(sub_line[1]);
            }
            else {
                cout << line_cnt << ": " << "Can not deallocate" << endl;
            }

        }

        else if (regex_search(line, sub_line, reg_alloc)) {
            cout << line_cnt << ": Alloc" << endl;

            Obj *newObj = new Obj();

            newObj->ptr = (u8*) w_malloc(stoi(sub_line[2]));
            if (newObj->ptr == nullptr) {
                cout << line_cnt << ": " << "Can not reserve memory" << endl;
                delete(newObj);
                LOG();
                cout << endl;
                continue;
            }
            newObj->refCount++;
            if (variables.find(sub_line[1]) != variables.end()) {
                if (variables[sub_line[1]].child != nullptr) {
                    if (!(--variables[sub_line[1]].child->refCount)) {
                        w_free(variables[sub_line[1]].child->ptr);
                        delete(variables[sub_line[1]].child);
                    }
                }
                variables[sub_line[1]].refCount--;
            }
            variables[sub_line[1]].child = newObj; // положить новый obj, заполнив ребёнка
            variables[sub_line[1]].refCount++;

        }

        else if (regex_search(line, sub_line, reg_assign)) {
            cout << line_cnt << ": " << "Assign" << endl;

            if (variables.find(sub_line[2]) != variables.end()) {
                this->update(sub_line[2], sub_line[1]);
            }
            else {
                cout << line_cnt << ": " << "Can not do the assignation" << endl;
            }
        }
        else {
            continue;
        }

        LOG();
        cout << endl;
    }
}

void Interpreter::read_files(const vector<string> &files){
    vector<string> strings_from_file;
    for (auto file : files) {
        ifstream fin(file);
        if (!fin.is_open()) {
            cout << "Can not open file: " << file << endl;
            continue;
        }

        cout << "FILE: " << file << endl;
        string line;
        while (getline(fin, line)) {
            strings_from_file.push_back(line);
        }
        this->interpret(strings_from_file);
        this->clear();
        strings_from_file.clear();
    }
}


void Interpreter::clear() {
    variables.clear();
    delete_all_memory();
}

int main(int argc, char *argv[]) {

    Interpreter our_interpreter;
    vector<string> files;

    for (int i = 1; i < argc; i++) {
        files.push_back(argv[i]);
    }

    our_interpreter.read_files(files);

    return 0;
}
// qapplication в мэйне

