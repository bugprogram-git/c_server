#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
typedef struct
{
    int local_socket;
    struct sockaddr_in local_addr;
    socklen_t len;
} local_server;
typedef struct
{
    int client_socket;
    struct sockaddr_in client_addr;
    char buffer[1024];
    pthread_t thread_id;
} local_client;
int thread_recv_messge(void *argv)
{
    local_client *local_client1 = (local_client *)argv;
    while (recv(local_client1->client_socket, local_client1->buffer, sizeof(local_client1->buffer), 0) > 0)
    {
        printf("recv from client %s:%d---->%s\n", inet_ntoa(local_client1->client_addr.sin_addr), ntohs(local_client1->client_addr.sin_port), local_client1->buffer);
    }
    printf("client---%s:%d----->disconnected!\n", inet_ntoa(local_client1->client_addr.sin_addr), ntohs(local_client1->client_addr.sin_port));
    free(argv);
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("two argument is require!\n");
        return 0;
    }
    local_server local_server1;
    local_server1.local_socket = socket(AF_INET, SOCK_STREAM, 0);
    local_server1.local_addr.sin_family = AF_INET;
    local_server1.local_addr.sin_addr.s_addr = inet_addr(argv[1]);
    local_server1.local_addr.sin_port = htons(atoi(argv[2]));
    local_server1.len = sizeof(struct sockaddr_in);
    if (bind(local_server1.local_socket, (struct sockaddr *)&local_server1.local_addr, local_server1.len) < 0)
    {
        printf("bind the local port is fail!\n");
        return 0;
    }
    if (listen(local_server1.local_socket, 5) < 0)
    {
        printf("listen the local port is fail!");
        return 0;
    }
    while (1)
    {
        printf("waiting the client connect....................\n");
        local_client *local_client1 = (local_client *)malloc(sizeof(local_client));
        local_client1->client_socket = accept(local_server1.local_socket, (struct sockaddr *)&local_client1->client_addr, &local_server1.len);
        if (local_client1->client_socket >= 0)
        {
            printf("connect the client %s:%d\n", inet_ntoa(local_client1->client_addr.sin_addr), ntohs(local_client1->client_addr.sin_port));
            pthread_create(local_client1->thread_id, NULL, (void *)thread_recv_messge, (void *)local_client1);
        }
    }
}
