#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
int create_bind_local_socket(struct sockaddr_in *remote_addr) //创建本地套接字并绑定端口l连接远程服务器
{
    int local_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (local_socket == -1)
    {
        printf("create the local_socket is fail!\n");
        return 0;
    }
    int ret = connect(local_socket, (struct sockaddr *)remote_addr, sizeof(*remote_addr));
    if (ret == -1)
    {
        printf("connect the remote server is fail!\n");
        return 0;
    }
    printf("connect the remote server is successful!\n");
    return local_socket;
}
int main()
{
    char buff[1024];
    struct sockaddr_in *remote_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    remote_addr->sin_family = AF_INET;
    remote_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    remote_addr->sin_port = htons(10000);
    int local_socket = create_bind_local_socket(remote_addr);
    printf("successful!\n");
    while (1)
    {
        printf("please input the string that you want to send to the remote server:");
        scanf("%s", buff);
        int ret = send(local_socket, buff, strlen(buff), 0); //发送数据
        if (ret <= 0)
        {
            printf("send message to server is fail!");
            return 0;
        }
        printf("send message to serve is successful!\n");
        ret = recv(local_socket, buff, sizeof(buff), 0);
        if (ret <= 0)
        {
            printf("recv from server message is fail!");
            return 0;
        }
        printf("recv from server:%s\n", buff);
    }
    close(local_socket);
    return 0;
}
