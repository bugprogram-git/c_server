#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
struct local_server
{
    int local_socket;
    struct sockaddr_in local_addr;
    socklen_t len;
};
struct client_socket
{
    int client_socket;
    struct sockaddr_in client_addr;
    char buffer[1024];
};
struct local_server init_socket(char **argv)
{
    struct local_server local_server1;
    local_server1.local_socket = socket(AF_INET, SOCK_STREAM, 0);
    local_server1.local_addr.sin_family = AF_INET;
    local_server1.local_addr.sin_addr.s_addr = inet_addr(argv[1]);
    local_server1.local_addr.sin_port = htons(atoi(argv[2]));
    local_server1.len = sizeof(struct sockaddr_in);
    return local_server1;
} //init local socket
void thread_send_message(void *client1) //declare send message thread function
{
    struct client_socket *client = (struct client_socket *)client1;
    bzero(client->buffer, strlen(client->buffer));
    while (1)
    {
        //printf("please input the message that you want to send to the client:");
        scanf("%s", client->buffer);
        if (send(client->client_socket, client->buffer, sizeof(client->buffer), 0) > 0)
        {
            printf("send message to ->%s:%d successful!\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
            bzero(client->buffer, strlen(client->buffer));
        }
    }
}
void thread_recv_message(void *client1) //declare recv message thread function
{
    struct client_socket *client = (struct client_socket *)client1;
    bzero(client->buffer, strlen(client->buffer));
    while (recv(client->client_socket, client->buffer, sizeof(client->buffer), 0) > 0)
    {
        printf("recv from client->%s:%d-->%s\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port), client->buffer);
        bzero(client->buffer, strlen(client->buffer));
    }
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("two argument is require!\n");
        return 0;
    }
    struct local_server local_server1 = init_socket(argv);
    if (bind(local_server1.local_socket, (struct sockaddr *)&local_server1.local_addr, local_server1.len) < 0) //bind ip address and port
    {
        printf("bind the local address and port is fail\n");
        return 0;
    }
    if (listen(local_server1.local_socket, 5) < 0) //listen
    {
        printf("listen local address and port is fail!\n");
        return 0;
    }
    struct client_socket client_socket1;
    bzero(client_socket1.buffer, sizeof(client_socket1.buffer));
    printf("listening........................\n");
    client_socket1.client_socket = accept(local_server1.local_socket, (struct sockaddr *)&client_socket1.client_addr, &local_server1.len);
    if (client_socket1.client_socket >= 0)
    {
        printf("accept a cilent connected-->%s:%d\n", inet_ntoa(client_socket1.client_addr.sin_addr), ntohs(client_socket1.client_addr.sin_port));
        pthread_t send_id, recv_id;
        pthread_create(&send_id, NULL, (void *)thread_send_message, (void *)&client_socket1); //create thread for send message
        pthread_create(&recv_id, NULL, (void *)thread_recv_message, (void *)&client_socket1); //create thread for recv message
        pthread_join(recv_id, NULL);
        pthread_join(send_id, NULL);
    }
    //pthread_destroy(NULL);
}