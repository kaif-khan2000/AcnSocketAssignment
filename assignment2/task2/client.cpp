#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#define SIZE 1024

using namespace std;
void write_file(char *filename, int sockfd)
{
    printf("\nEntered write file\n");
    int n;
    FILE *fp;
    char buffer[SIZE];

    fp = fopen(filename, "w");
    // n = recv(sockfd, buffer, SIZE, 0);
    //  fprintf(fp, "%s", buffer);

    while (1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        printf("\n%d\n", n);
        printf("Buffer %s \n ", buffer);
        if (n <= 0)
        {
            break;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    fclose(fp);
    return;
}

int createClient()
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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(1234);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        cout << "Error in connecting1" << endl;
        exit(0);
    }
    cout << "Socket connected" << endl;

    return sockfd;
}

// creating a file client

int main()
{
    int sockfd = createClient();
    // send filename to server
    char filename[1024], path[1024];
    cout << "Enter path of directory: ";
    cin >> path;
    cout << "Enter filename: ";
    cin >> filename;
    strcat(path, filename);

    send(sockfd, path, 1024, 0);
    // receive status from server
    char status[1024];
    recv(sockfd, status, 1024, 0);
    if (strcmp(status, "File not found") == 0)
    {
        cout << "File not found" << endl;
        close(sockfd);
        exit(0);
    }

    write_file(filename, sockfd);
    // FILE *fp = fopen(filename, "w");
    // if (fp == NULL)
    // {
    //     cout << "Error in opening file" << endl;
    //     exit(0);
    // }

    // while (true)
    // {
    //     char buffer[1024];
    //     memset(buffer, 0, 1024);
    //     if ((recv(sockfd, buffer, 1024, 0)) <= 0)
    //     {
    //         break;
    //     }
    //     cout << buffer << endl;
    //     fprintf(fp, "%s", buffer);
    // }
    // fclose(fp);
    close(sockfd);
    return 0;
}