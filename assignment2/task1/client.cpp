#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>

using namespace std;
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
    cout << "Enter filename: ";
    cin >> filename;
    send(sockfd, filename, 1024, 0);
    //receive status from server
    char status[1024];
    recv(sockfd, status, 1024, 0);
    if (strcmp(status, "File not found") == 0) {
        cout << "File not found" << endl;
        close(sockfd);
        exit(0);
    }
    FILE *fp = fopen("file1.txt", "w");
    if (fp == NULL) {
        cout << "Error in opening file" << endl;
        exit(0);
    }
    
    
    while (true) {
        char buffer[1024];
        memset(buffer, 0, 1024);
        if((recv(sockfd, buffer, 1024, 0)) <= 0){
            break;
        }
        cout << buffer << endl;
        //add /0 at the end of buffer
        fputs(buffer, fp);
    }
    fclose(fp);
    close(sockfd);
    return 0;
}