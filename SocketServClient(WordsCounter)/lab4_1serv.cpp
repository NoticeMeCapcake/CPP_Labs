#include <stdio.h>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int count_words(char *line) {
    bool alpha = false, space = false;
    int cnt = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (isalnum(line[i])) {
            alpha = true;
            space = false;
        }
        else if (isspace(line[i])) {
            if (alpha) {
                cnt++;
            }
            space = true;
            alpha = false;
        }
    }
    if (alpha) {
        cnt++;
    }
    return cnt;
}

int main()
{
    int sock, listen_soc;
    struct sockaddr_in addr;
    char msg_rcv[1000];
    int bytes_read, total = 0;
    char msg_snd[100];

    listen_soc = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_soc < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listen_soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listen_soc, 1);

    while(1)
    {
        sock = accept(listen_soc, (struct sockaddr *)NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }


        bytes_read = recv(sock, msg_rcv, 1000, 0);
        if(bytes_read <= 0) break;

        snprintf(msg_snd, sizeof(msg_snd), "%d", count_words(msg_rcv));

        send(sock, msg_snd, sizeof(msg_snd), 0);


        close(sock);
    }

    return 0;
}
