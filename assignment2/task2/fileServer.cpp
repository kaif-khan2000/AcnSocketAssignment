#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <sys/stat.h>

#define SIZE 1024
using namespace std;
void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};
    
    while ((n=fread(data, sizeof(char) , 1024, fp))>0)
    {
        if (send(sockfd, data, n, 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
    printf("File is sucessfully sent\n");
    close(sockfd);
}

int createASocket()
{
    // socket creation
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cout << "Error in creating socket" << endl;
        exit(0);
    }
    cout << "Socket created" << endl;
    // socket binding
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(1234);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        cout << "Error in binding" << endl;
        exit(0);
    }

    cout << "Socket binded" << endl;
    return sockfd;
}

void handleClient(int sock)
{
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    while (1)
    {
        int sockfd = accept(sock, (struct sockaddr *)&cliaddr, &clilen);
        if (sockfd < 0)
        {
            cout << "Error in accepting" << endl;
            exit(0);
        }
        cout << "Socket accepted" << endl;
        char filename[1024];
        recv(sockfd, filename, 1024, 0);
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            char status[1024] = "File not found";
            cout << "Error in opening file " << filename << endl;
            send(sockfd, status, 1024, 0);
            close(sockfd);
            continue;
        }
        struct stat file_stats;
        stat(filename, &file_stats);
        char status[1024];
        sprintf(status,"%d",(int)file_stats.st_size);
        send(sockfd,status,1024,0);
        send_file(fp, sockfd);
        // char status[1024] = "File found";
        // send(sockfd, status, 1024, 0);
        // char buffer[1024];
        // while (fgets(buffer, 1024, fp) != NULL)
        // {
        //     cout << buffer << endl;
        //     send(sockfd, buffer, strlen(buffer), 0);
        //     bzero(buffer, 1024);
        // }
        fclose(fp);
        // close(sockfd);
    }
}
// creating a file server
int main()
{
    int sock = createASocket();

    listen(sock, 5);
    cout << "Socket listening" << endl;
    thread clients[5];

    for (int i = 0; i < 5; i++)
    {
        clients[i] = thread(handleClient, sock);
    }

    for (int i = 0; i < 5; i++)
    {
        clients[i].join();
    }
    return 0;
}