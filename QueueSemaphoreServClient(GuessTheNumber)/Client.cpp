#include <stdio.h>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>


struct MsgType {
    long type;
    unsigned int shift;
    bool last;
    char msg[1000];
};



using namespace std;

int main()
{
    char msg[1000];
    char rcv_data[100];
    struct sockaddr_in s_addr;
    string u_ans;
    MsgType sock_msg;


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("Can not make a socket\n");
        exit(-1);
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(3425);
    s_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
    {
        perror("Connection error\n");
        exit(-2);
    }

    cout << "This program can do two tasks.\n" <<
                "First one is encryptor by Caesar cipher.\n" <<
                "Second - game \"Guess the number\"" << endl;
    while (1) {
        cout << "Enter a number [1\\2] of the task you want to execute or type \"exit\"" << endl;
        cin >> u_ans;
        while (getchar() != '\n');

        if (u_ans == "1") {
            string path;
            cout << "This is encryptor." << endl;
            FILE *in_file = nullptr;
            while (1) {
                cout << "Enter the path to the file:" << endl;
                getline(cin, path);
                in_file = fopen(path.c_str(), "r");
                if (!in_file) {
                    cout << "Can\'t open this file. Please retype" << endl;
                    continue;
                }
                break;
            }
            cout << "Enter positive or negative number for Caesar cipher:" << endl;
            int shift = 0;
            size_t pos_shift = 0;
            string in_shift;
            while (1) {
                cin >> in_shift;
                while (getchar() != '\n');
                try {
                    shift = stoi(in_shift, &pos_shift);
                }
                catch(invalid_argument) {
                    pos_shift = 0;
                }
                if (pos_shift and shift)
                    break;
                cout << "Wrong value or format. Retype." << endl;
            }

            sock_msg.shift = shift;
            sock_msg.type = 0;
            sock_msg.last = false;

            while (1) {
                int bytes_read = fread(sock_msg.msg, sizeof(char), 999, in_file);
                sock_msg.msg[bytes_read] = '\0';
                if (feof(in_file)) {
                    sock_msg.last = true;
                }
                if (bytes_read < 999) {
                    if (!feof(in_file)) {
                        cout << "Something went wrong while file were read." << endl;
                        exit(-7);
                    }
                }
                if (send(sock, &sock_msg, sizeof(sock_msg), 0) < 0) {
                    perror("Send error");
                    exit(-3);
                }
                if (sock_msg.last)
                    break;
            }
            fclose(in_file);
            sock_msg.type = 1;
            FILE *res;
            res = fopen("Result", "w");
            if (!res) {
                cout << "Unexpected error, can\'t create result file" << endl;
                exit(-6);
            }
            sock_msg.last = false;
            while (1) {
                recv(sock, &sock_msg, sizeof(sock_msg), 0);
                fprintf(res, "%s", sock_msg.msg);
                if (sock_msg.last) {
                    break;
                }
            }
            fclose(res);
            cout << "Program executed successfully. Check file \"Result.txt\" for result" << endl;
            exit(0);
        }
        else if (u_ans == "2") {
            cout << "This is the \"Guess the number\" game." << endl;
            cout << "You should guess the right number from 1 to 10000" << endl;
            sock_msg.type = 2;
            sock_msg.last = false;
            sock_msg.shift = 0;
            unsigned int random_num;
            strcpy(sock_msg.msg, "create");
            if (send(sock, &sock_msg, sizeof(sock_msg), 0) < 0) {
                perror("Send error");
                exit(-3);
            }
            recv(sock, &sock_msg, sizeof(sock_msg), 0);
            random_num = sock_msg.type;
            string num_s;
            int attempts_num = 0;
            while(1) {
                int in_num = 0;
                size_t pos = 0;
                cout << "Enter the guessing number:" << endl;
                cin >> num_s;
                while (getchar() != '\n');
                try {
                    in_num = stoi(num_s, &pos);
                }
                catch(invalid_argument) {
                    pos = 0;
                }
                if (in_num <= 0 or pos != num_s.length() or pos == 0) {
                    cout << "Wrong format. Please retype." << endl;
                    continue;
                }
                attempts_num++;
                sock_msg.msg[0] = '\0';
                sock_msg.shift = in_num;
                sock_msg.type = random_num;
                if (send(sock, &sock_msg, sizeof(sock_msg), 0) < 0) {
                    perror("Send error");
                    exit(-3);
                }
                recv(sock, &sock_msg, sizeof(sock_msg), 0);
                if (string(sock_msg.msg) == "low") {
                    cout << "Your number is too small!" << endl;
                }
                else if (string(sock_msg.msg) == "high") {
                    cout << "Your number is too large!" << endl;
                }
                else {
                    cout << "Congratulations! You have finally won!\nNumber of attempts: " << attempts_num << endl;
                    exit(0);
                }
            }
        }
        else if (u_ans == "exit") {
            cout << "Shutting down..." << endl;
            cout << "Goodbye!" << endl;
            exit(0);
        }
        else {
            cout << "Wrong format of the answer. Please retype." << endl;
            continue;
        }
    }

    printf("Number of words: %s\n", "sasasfas");
    close(sock);

    return 0;
}
