#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
//local struct socket type
struct local_client
{
    int local_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    socklen_t len;
};
// init the local socket
struct local_client init__socket(char **argv)
{
    struct local_client local_client1;
    bzero(local_client1.buffer, sizeof(local_client1.buffer));
    local_client1.local_socket = socket(AF_INET, SOCK_STREAM, 0);
    local_client1.server_addr.sin_family = AF_INET;
    local_client1.server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    local_client1.server_addr.sin_port = htons(atoi(argv[2]));
    local_client1.len = sizeof(struct sockaddr_in);
    return local_client1;
}
void thread_send_message(void *client1) //declare send message thread function
{
    struct local_client *client = (struct local_client *)client1;
    bzero(client->buffer, strlen(client->buffer));
    while (1)
    {
        //printf("please input the message that you want to send to the server:");
        scanf("%s", client->buffer);
        if (send(client->local_socket, client->buffer, strlen(client->buffer), 0) > 0)
        {
            printf("send message to-->%s:%d successful!\n", inet_ntoa(client->server_addr.sin_addr), ntohs(client->server_addr.sin_port));
        }
    }
}
void thread_recv_message(void *client1) //declare recv message thread function
{
    struct local_client *client = (struct local_client *)client1;
    bzero(client->buffer, strlen(client->buffer));
    while (recv(client->local_socket, client->buffer, sizeof(client->buffer), 0) > 0)
    {
        printf("recv from the server->%s:%d-->%s\n", inet_ntoa(client->server_addr.sin_addr), ntohs(client->server_addr.sin_port), client->buffer);
    }
}
int file_transfer(struct local_client *client, char *filename) //transfer file function
{
    bzero(client->buffer, sizeof(client->buffer));
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        return 0;
    }
    char *data;
    while (!feof)
    {
        fread(data, 64, 16, fp);
        send(client->local_socket, client->buffer, sizeof(client->buffer), 0);
    }
    return 1;
}
int main(int argc, char **argv) //main function
{
    struct local_client local_client1 = init__socket(argv);
    if (connect(local_client1.local_socket, (struct sockaddr *)&local_client1.server_addr, local_client1.len) >= 0) //connection the server
    {
        printf("connected the server->%s:%d\n", inet_ntoa(local_client1.server_addr.sin_addr), ntohs(local_client1.server_addr.sin_port));
        pthread_t send_id, recv_id;
        pthread_create(&send_id, NULL, (void *)thread_send_message, (void *)&local_client1); //create the send message thread
        pthread_create(&recv_id, NULL, (void *)thread_recv_message, (void *)&local_client1); //create the recv message thread
        pthread_join(recv_id, NULL);
        pthread_join(send_id, NULL);
    }
    //pthread_destroy(NULL);
}