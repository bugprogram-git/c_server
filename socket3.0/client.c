#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
typedef struct
{
    int local_socket;
    struct sockaddr_in server_addr;
    socklen_t len;
} local_client;
int thread_send_message(void *argv)
{
    local_client *local_client1 = (local_client *)argv;
    char buffer[1024];
    while (1)
    {
        scanf("%s", buffer);
        if (send(local_client1->local_socket, buffer, sizeof(buffer), 0) > 0)
        {
            printf("send message %s:%d is successful!\n", inet_ntoa(local_client1->server_addr.sin_addr), ntohs(local_client1->server_addr.sin_port));
        }
    }
}
int thread_recv_message(void *argv)
{
    local_client *local_client1 = (local_client *)argv;
    char buffer[1024];
    while (recv(local_client1->local_socket, buffer, sizeof(buffer), 0) > 0)
    {
        printf("recv from the client %s:%d---->%s\n", inet_ntoa(local_client1->server_addr.sin_addr), ntohs(local_client1->server_addr.sin_port),buffer);
    }
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("two argument is require!\n");
        return 0;
    }
    local_client local_client1;
    local_client1.local_socket = socket(AF_INET, SOCK_STREAM, 0);
    local_client1.server_addr.sin_family = AF_INET;
    local_client1.server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    local_client1.server_addr.sin_port = htons(atoi(argv[2]));
    local_client1.len = sizeof(struct sockaddr_in);
    if (connect(local_client1.local_socket, (struct sockaddr *)&local_client1.server_addr, local_client1.len) < 0)
    {
        printf("connect the server is fail!\n");
        return 0;
    }
    printf("connected to server->%s:%d\n",inet_ntoa(local_client1.server_addr.sin_addr),ntohs(local_client1.server_addr.sin_port));
    pthread_t send_id, recv_id;
    pthread_create(&send_id, NULL, (void *)thread_send_message, (void *)&local_client1);
    pthread_create(&recv_id, NULL, (void *)thread_recv_message, (void *)&local_client1);
    pthread_join(send_id, NULL);
    pthread_join(recv_id, NULL);
}
