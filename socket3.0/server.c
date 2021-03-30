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
} local_client;
int thread_send_message(void *argv)
{
    char buffer[1024];
    local_client *local_client1 = (local_client *)argv;
    while (1)
    {
        scanf("%s", buffer);
        if (send(local_client1->client_socket, buffer, sizeof(buffer), 0) > 0)
        {
            printf("send message %s:%d is successful!\n", inet_ntoa(local_client1->client_addr.sin_addr), ntohs(local_client1->client_addr.sin_port));
        }
    }
}
int thread_recv_message(void *argv)
{
    char buffer[1024];
    local_client *local_client1 = (local_client *)argv;
    while (recv(local_client1->client_socket, buffer, sizeof(buffer), 0) > 0)
    {
        printf("recv from the client %s:%d---->%s\n", inet_ntoa(local_client1->client_addr.sin_addr), ntohs(local_client1->client_addr.sin_port),buffer);
    }
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
        printf("bind the local_port is fail!\n");
        return 0;
    }
    if (listen(local_server1.local_socket, 5) < 0)
    {
        printf("listen the local port is fail!\n");
        return 0;
    }
    printf("waiting the clinet connected......................\n");
    local_client local_client1;
    local_client1.client_socket = accept(local_server1.local_socket, (struct sockaddr *)&local_client1.client_addr, &local_server1.len);
    if (local_client1.client_socket < 0)
    {
        printf("accept the client is fail!\n");
        return 0;
    }
    printf("connected->%s:%d\n", inet_ntoa(local_client1.client_addr.sin_addr), ntohs(local_client1.client_addr.sin_port));
    pthread_t *send_id = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_t *recv_id = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_create(send_id, NULL, (void *)thread_send_message, (void *)&local_client1);
    pthread_create(recv_id, NULL, (void *)thread_recv_message, (void *)&local_client1);
    pthread_join(*send_id, NULL);
    pthread_join(*recv_id, NULL);
}
