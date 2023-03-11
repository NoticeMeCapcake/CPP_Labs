#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
    if (argc != 5){
        printf("Wrong amount of arguments (four is needed)\n");
        exit(-1);
    }

    struct sockaddr_in in_addr, out_addr;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    if (!strcmp(argv[1], "-a")) {
        char addr_line[sizeof(argv[2])];
        strcpy(addr_line, argv[2]);
        char *splited = strtok(addr_line, ":");
        if (!inet_aton(splited, &(in_addr.sin_addr))) {
            printf("Wrong addres\n");
            exit(-2);
        }
        splited = strtok(NULL, ":");
        in_addr.sin_port = htons(atoi(splited));
    }
    else {
        printf("Wrong key (should be \"-a\")\n");
        exit(-3);
    }

    in_addr.sin_family = AF_INET;
    out_addr.sin_family = AF_INET;
    out_addr.sin_addr = in_addr.sin_addr;

    char name[60];
    if (!strcmp(argv[3], "-u")) {
        strcpy(name, argv[4]);
    }
    else {
        printf("Wrong key (should be \"-u\")\n");
        exit(-4);
    }

    printf("Enter friend\'s port\n>> ");
    int out_port;
    scanf("%d", &out_port);
    getchar();
    out_addr.sin_port = htons(out_port);

    listen(sock, 2);

    printf("Notice that max length of message is 938 symbols\n");

    pid_t pid = fork();
    char input[938], quit_msg[200], msg[1000];
    char msg_rcv[1000];
    int in_addr_size = sizeof(in_addr);
    switch (pid) {
    case -1:
        printf("Oops, looks like we\'ve got some issues\n");
        close(sock);
        exit(-6);
    case 0:
        printf("Start of the chat\n");
        while(1) {
            fgets(input, sizeof(input), stdin);
            if (!strcmp(input, "quit!\n")) {

                printf("Shutting down...\n");
                snprintf(msg, sizeof(msg), "User %s has left the chat\n", name);
                sendto(sock, msg, sizeof(msg), 0, (struct sockaddr *)&out_addr, sizeof(out_addr));
                sendto(sock, input, sizeof(msg), 0, (struct sockaddr *)&in_addr, sizeof(in_addr));
                exit(0);
            }
            snprintf(msg, sizeof(msg), "%s: %s", name, input);
            sendto(sock, msg, sizeof(msg), 0, (struct sockaddr *)&out_addr, sizeof(out_addr));
        }
    default:
        while(1) {
            recvfrom(sock, msg_rcv, sizeof(msg_rcv), 0, (struct sockaddr *)&in_addr, &in_addr_size);
            if (!strcmp(msg_rcv, "quit!\n")) {
                close(sock);
                printf("Success! Good bye!\n");
                exit(0);
            }
            printf("%s%s", msg_rcv, ((msg_rcv[strlen(msg_rcv) - 1] == '\n') ? "" : "\n"));
        }
    }
    close(sock);

    return 0;
}
