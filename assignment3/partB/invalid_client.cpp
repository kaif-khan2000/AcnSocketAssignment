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
    //printf("\nEntered write file\n");
    int n;
    FILE *fp;
    char buffer[SIZE];

    //opening file in write mode
    fp = fopen(filename, "wb");
    int remaining_data = length, received_data_len;
    
    //writing data into the file.
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
    // socket connecting to server
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

    //sending access token
    char buffer[1024];
    sprintf(buffer, "%d", (fileServer.accessToken+1));
    send(sockfd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);

    //receive access token to be correct or not.
    int n = recv(sockfd, buffer, 1024, 0);
    buffer[n] = '\0';
    if (strcmp(buffer, "Token invalid") == 0)
    {
        cout << "Access Denied invalid token" << endl;
        return -1;
    }
    else
    {
        cout << "Access Granted" << endl;
    }
    return sockfd;
}

// creating a file client

int main()
{

    //creating a UDP socket
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

    //sending a request to the bootstrapserver
    char buffer[1024];
    sprintf(buffer,"2@get");
    sendto(sock2, buffer, strlen(buffer), 0, (sockaddr *)&peer, len);
    bzero(buffer, 1024);

    //receiving the no.of servers
    recvfrom(sock2, buffer, 1024, 0, (sockaddr *)&peer, &len);
    int n = atoi(buffer);
    cout << "Number of fileServers: " << n << endl;
    string fileServers[n];

    //receiving the fileServers information
    for (int i = 0; i < n; i++)
    {
        bzero(buffer, 1024);
        recvfrom(sock2, buffer, 1024, 0, (sockaddr *)&peer, &len);
        Message msg = Message(buffer);
        fileServers[i] = msg.msg;
        cout << "[" << (i+1) << "] " << fileServers[i] << endl;
    }

    //reading fileserver number from user
    cout << "Enter the fileServer number: ";
    int fileServerNumber;
    cin >> fileServerNumber;
    fileServerNumber--;
    
    //creating a fileServer object
    Server fileServer = Server(fileServers[fileServerNumber]);

    //creating a client, i.e connecting to the fileServer
    int sockfd = createClient(fileServer);
    if (sockfd == -1)
    {
        return 0;
    }
    // send filename to server
    char filename[1024];
    char path[1024];
    cout << "Enter path of file: ";
    cin >> path;
    char temp[1024];

    //fetching filename from path
    strcpy(temp, path);
    char *token = strtok(temp, "/");
    while (token != NULL)
    {
        strcpy(filename, token);
        token = strtok(NULL, "/");
    }
    cout << "Filename:" << filename << endl;
    
    //sending path to fileserver
    send(sockfd, path, strlen(path), 0);
    
    //check ig the filetype is valid or not
    bzero(buffer, 1024);
    n = recv(sockfd, buffer, 1024, 0);
    buffer[n] = '\0';
    if (strcmp(buffer, "Invalid file type") == 0)
    {
        cout << "Invalid file type" << endl;
        return 0;
    }
    else
    {
        cout << "Valid file type" << endl;
    }

    // receive status of file from server
    char status[1024];
    recv(sockfd, status, 1024, 0);
    if (strcmp(status, "File not found") == 0)
    {
        cout << "File not found" << endl;
        close(sockfd);
        exit(0);
    }

    //if file is found, receive length of file so convert into int
    int length = atoi(status);

    //receive file from server and write to file
    write_file(filename, length, sockfd);

    //close the socket
    close(sockfd);
    return 0;
}