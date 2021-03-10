#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
int listen_local_socket()
{
    int local_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in *local_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    local_addr->sin_family = AF_INET;
    local_addr->sin_port = htons(10000);
    local_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len = sizeof(struct sockaddr_in); //初始化本地监听套接字
    int ret = bind(local_socket, (struct sockaddr *)local_addr, len);
    if (ret == -1)
    {
        printf("bind the local port is fail!\n");
        return -1;
    }
    int ret1 = listen(local_socket, 5);
    if (ret1 == -1)
    {
        printf("listen the local_socket is fail!\n");
        return -1;
    }
    return local_socket;
}
int main()
{
    char buff[1024];
    memset(buff, 0, 1024); //定义数据收发缓冲区
    struct sockaddr_in *client_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    socklen_t len = sizeof(struct sockaddr_in);
    int local_socket = listen_local_socket(); //创建套接字并监听本地端口
    if (local_socket == -1)
    {
        printf("fail!");
        return 0;
    }
    printf("listening...................\n");
    int client_socket = accept(local_socket, (struct sockaddr *)client_addr, &len);
    printf("connected->%s:%d\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    while (1)
    {
        printf("wait for the client send to message\n");
        int ret = recv(client_socket, buff, sizeof(buff), 0); //收到来自客户端的信息
        if (ret <= 0)
        {
            printf("recv the client message is fail!");
            return 0;
        }
        printf("%s\n", buff);
        strcpy(buff, "ok");
        ret = send(client_socket, buff, sizeof(buff), 0);
        if (ret <= 0)
        {
            printf("send ok to client is fail");
            return 0;
        }
    }
    printf("complete !");
    return 0;
}
