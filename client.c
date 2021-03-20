#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <string.h>
#include <sys/types.h>
typedef struct local_client
{
    int local_socket;
    struct sockaddr_in *server_addr;
    socklen_t len;
    char buffer[1024];
} local_client;
local_client connect_server(char **addr)
{
    local_client local_client1;
    local_client1.local_socket = socket(AF_INET, SOCK_STREAM, 0); //创建socket
    local_client1.server_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    local_client1.server_addr->sin_family = AF_INET;
    local_client1.server_addr->sin_addr.s_addr = inet_addr(addr[1]);
    local_client1.server_addr->sin_port = htons(atoi(addr[2])); //初始化服务端ip以及端口
    local_client1.len = sizeof(struct sockaddr_in);
    return local_client1;
}
int main(int argc, char **argv)
{
    local_client local_client1 = connect_server(argv);
    if (connect(local_client1.local_socket, (struct sockaddr *)local_client1.server_addr, local_client1.len) >= 0) //连接服务端
    {
        printf("connected the remote server->%s:%d is successful!\n", inet_ntoa(local_client1.server_addr->sin_addr), ntohs(local_client1.server_addr->sin_port));
        while (1)
        {
            printf("please input the message that you want to send to server:");
            scanf("%s", local_client1.buffer);
            if (send(local_client1.local_socket, local_client1.buffer, strlen(local_client1.buffer), 0) > 0) //发送数据到服务端
            {
                printf("send message to serve is successful!\n");
            }
            bzero(local_client1.buffer, strlen(local_client1.buffer));
        }
    }
}