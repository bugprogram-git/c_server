#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
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
int thread_send_file(void *argv)
{
    local_client *local_client1 = (local_client *)argv;
    printf("connected----->%s:%d", inet_ntoa(local_client1->client_addr.sin_addr), ntohs(local_client1->client_addr.sin_port));
    while (1)
    {
        char buffer[1024];
        FILE *file = fopen("/home/arch/index.html", "r");
        while (!feof(file))
        {
            fread(buffer, 1, sizeof(buffer), file);
            send(local_client1->client_socket, buffer, sizeof(buffer), 0);
        }
    }
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
        perror("bind the local port is fail!");
    }
    if (listen(local_server1.local_socket, 5) < 0)
    {
        perror("listen the local port is fail!");
    }
    local_client *local_client1;
    while (1)
    {
        local_client1 = (local_client *)malloc(sizeof(local_client));
        printf("waiting for the client connect...................\n");
        local_client1->client_socket = accept(local_server1.local_socket, (struct sockaddr *)&local_client1->client_addr, &local_server1.len);
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        pthread_create(&local_client1->thread_id, NULL, (void *)thread_send_file, (void *)local_client1);
        pthread_join(local_client1->thread_id, NULL);
    }
}
