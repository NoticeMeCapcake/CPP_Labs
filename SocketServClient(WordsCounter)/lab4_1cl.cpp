#include <stdio.h>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

int sendall(int s, char *buf, int len, int flags)
{
    int total = 0;
    int bytes_sent = 0;

    while(total < len)
    {
        bytes_sent = send(s, buf+total, len-total, flags);
        if (bytes_sent == -1)
            break;
        total += bytes_sent;
    }

    return ((bytes_sent == -1) ? -1 : total);
}

int main()
{
    char msg[1000];
    char rcv_data[100];
    struct sockaddr_in s_addr;

    std::cout << "Input string:\n";
    fgets(msg, sizeof(msg), stdin);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("Can not make a socket\n");
        exit(-1);
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(3425); // или любой другой порт...
    s_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // внутри машины
    if(connect(sock, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
    {
        perror("Connection error\n");
        exit(-2);
    }
    // printf("%s\n", msg);
    if (send(sock, msg, sizeof(msg), 0) < 0) {
        perror("Send error\n");
        exit(-3);
    }
    recv(sock, rcv_data, sizeof(rcv_data), 0);

    printf("Number of words: %s\n", rcv_data);
    close(sock);

    return 0;
}
