#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int htconnect(char* domain, int port)
{
    int white_sock;
    struct hostent* site;
    struct sockaddr_in me;

    site = gethostbyname(domain);
    if(!site) return -2;

    white_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(white_sock < 0) return -1;

    memset(&me, 0, sizeof(struct sockaddr_in));
    memcpy(&me.sin_addr, site->h_addr_list[0], site->h_length);
    me.sin_family = AF_INET;
    me.sin_port = htons(port);

    if(connect(white_sock, (struct sockaddr*)&me, sizeof(struct sockaddr))<0)
    {
        return -1;
    }
    return white_sock;
}

int htsend(int sock, char* fmt,...)
{
    char BUF[1024];
    va_list argptr;
    va_start(argptr, fmt);
    vsprintf(BUF, fmt, argptr);
    va_end(argptr);
    printf("%s", BUF);
    return send(sock, BUF, strlen(BUF), 0);
}

void main(int argc, char *argv[])
{
    int black_sock;
    char bugs_bunny[3];
    //if(argc < 2)
    //{
    //    printf("Usage:\n\twww_client host\n");
    //    return;
    //}

    black_sock = htconnect("localhost", 4000);
    if(black_sock < 0)
    {
        printf("Socket Connect Error!\n");
        return;
    }

    htsend(black_sock, "GET / HTTP/1.0%c", 10);
    htsend(black_sock, "Host: %s%c",argv[1], 10);
    htsend(black_sock, "%c",10);

    while(read(black_sock, bugs_bunny, 1) > 0)
    {
        printf("%c",bugs_bunny[0]);
    }

    close(black_sock);
}

#if 0
int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char ch = 'A';

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("oops: client1");
        exit(1);
    }
    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);
}
#endif
