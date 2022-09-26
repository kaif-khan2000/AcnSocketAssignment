#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>

#define SIZE 1024
using namespace std;

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};

    while ((n = fread(data, sizeof(char), 1024, fp)) > 0)
    {
        if (send(sockfd, data, n, 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
    printf("File is sucessfully sent\n");
}

int createServer()
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

    // listening
    listen(sockfd, 5);

    cout << "Socket listening" << endl;
    // accepting
    return sockfd;
}
int acceptClient(int sockfd) {
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    if (newsockfd < 0)
    {
        cout << "Error in accepting" << endl;
        exit(0);
    }

    cout << "Socket accepted" << endl;

    return newsockfd;
}
// creating a file server
int main()
{
    int sockfd1 = createServer();
    while (true)
    {
        int sockfd = acceptClient(sockfd1);
        char filename[1024];
        recv(sockfd, filename, 1024, 0);
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            cout << "Error in opening file" << endl;
            char status[1024] = "File not found";
            send(sockfd, status, 1024, 0);
            close(sockfd);
            exit(0);
        }
        struct stat file_stats;
        stat(filename, &file_stats);
        char status[1024];
        sprintf(status, "%d", (int)file_stats.st_size);
        send(sockfd, status, 1024, 0);
        send_file(fp, sockfd);

        fclose(fp);
        close(sockfd);
    }
    return 0;
}