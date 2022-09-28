#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "datatypes.cpp"

#define SIZE 1024

using namespace std;
void write_file(char *filename, int length, int sockfd)
{
    printf("\nEntered write file\n");
    int n;
    FILE *fp;
    char buffer[SIZE];

    fp = fopen(filename, "wb");
    int remaining_data = length, received_data_len;
    while ((remaining_data > 0) && ((received_data_len = recv(sockfd, buffer, 1024, 0)) > 0))
    {
        fwrite(buffer, sizeof(char), received_data_len, fp);
        remaining_data -= received_data_len;
        printf("Received %d/%d bytes\n", (length - remaining_data), length);
    }

    return;
}

int createClient(Server fileServer)
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
    servaddr.sin_addr.s_addr = inet_addr(fileServer.ip);
    servaddr.sin_port = htons(fileServer.port);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        cout << "Error in connecting1" << endl;
        exit(0);
    }
    cout << "Socket connected" << endl;
    char buffer[1024];
    sprintf(buffer, "%d", fileServer.accessToken);
    send(sockfd, buffer, strlen(buffer), 0);

    return sockfd;
}

// creating a file client

int main()
{

    int sock2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock2 < 0)
    {
        cerr << "socket error" << endl;
        return 0;
    }
    sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1234);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len = sizeof(peer);
    char buffer[1024];
    sprintf(buffer,"2@get");
    sendto(sock2, buffer, strlen(buffer), 0, (sockaddr *)&peer, len);
    bzero(buffer, 1024);
    recvfrom(sock2, buffer, 1024, 0, (sockaddr *)&peer, &len);
    int n = atoi(buffer);
    cout << "Number of fileServers: " << n << endl;
    string fileServers[n];

    for (int i = 0; i < n; i++)
    {
        bzero(buffer, 1024);
        recvfrom(sock2, buffer, 1024, 0, (sockaddr *)&peer, &len);
        Message msg = Message(buffer);
        fileServers[i] = msg.msg;
        cout << "[" << (i+1) << "] " << fileServers[i] << endl;
    }

    cout << "Enter the fileServer number: ";
    int fileServerNumber;
    cin >> fileServerNumber;
    fileServerNumber--;
    
    Server fileServer = Server(fileServers[fileServerNumber]);

    int sockfd = createClient(fileServer);
    // send filename to server
    char filename[1024];
    char path[1024];
    cout << "Enter path of file: ";
    cin >> path;
    char temp[1024];
    strcpy(temp, path);
    char *token = strtok(temp, "/");
    while (token != NULL)
    {
        strcpy(filename, token);
        token = strtok(NULL, "/");
    }
    cout << "Filename:" << filename << endl;
    // cout << "Enter filename: ";
    // cin >> filename;
    // strcat(path, filename);

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

    int length = atoi(status);

    write_file(filename, length, sockfd);
    close(sockfd);
    return 0;
}