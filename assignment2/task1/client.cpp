#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#define SIZE 1024

using namespace std;
void write_file(char *filename,int length, int sockfd)
{
    printf("\nEntered write file\n");
    int n;
    FILE *fp;
    char buffer[SIZE];

    fp = fopen(filename, "w");
    int remaining_data = length, received_data_len;
    while ((remaining_data > 0) && ((received_data_len = recv(sockfd, buffer, 1024, 0)) > 0))
    {
        fwrite(buffer, sizeof(char), received_data_len, fp);
        remaining_data -= received_data_len;
        printf("Received %d/%d bytes\n", (length - remaining_data), length);
    }
    
    return;
}
int createClient() {
    //socket creation
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cout << "Error in creating socket" << endl;
        exit(0);
    }
    cout << "Socket created" << endl;
    //socket binding
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(1234);
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        cout << "Error in connecting1" << endl;
        exit(0);
    }
    cout << "Socket connected" << endl;
    
    return sockfd;
}


//creating a file client

int main() {
    int sockfd = createClient();
    //send filename to server
    char filename[1024];
    char path[1024];
    cout << "Enter path of file: ";
    cin >> path;
    char temp[1024];
    strcpy(temp,path);
    char *token = strtok(temp, "/");
    while (token != NULL)
    {
        strcpy(filename, token);
        token = strtok(NULL, "/");
    }
    cout << "Filename:" << filename << endl;
    
    send(sockfd, path, 1024, 0);
    //receive status from server
    char status[1024];
    recv(sockfd, status, 1024, 0);
    if (strcmp(status, "File not found") == 0) {
        cout << "File not found" << endl;
        close(sockfd);
        exit(0);
    }
    int length = atoi(status);
    write_file(filename,length, sockfd);

    close(sockfd);
    return 0;
}