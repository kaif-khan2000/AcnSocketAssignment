#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>

using namespace std;
int createServer() {
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
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(1234);
    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        cout << "Error in binding" << endl;
        exit(0);
    }

    cout << "Socket binded" << endl;

    //listening
    listen(sockfd, 5);

    cout << "Socket listening" << endl;
    //accepting
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
    if (newsockfd < 0) {
        cout << "Error in accepting" << endl;
        exit(0);
    }

    cout << "Socket accepted" << endl;
    
    return newsockfd;
}

//creating a file server
int main() {
    int sockfd = createServer();
    char filename[1024];
    recv(sockfd, filename, 1024, 0);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        cout << "Error in opening file" << endl;
        char status[1024] = "File not found";
        send(sockfd, status, 1024, 0);
        close(sockfd);
        exit(0);
    }
    char status[1024] = "File found";
    send(sockfd, status, 1024, 0);
    char buffer[1024];
    while (fgets(buffer, 1024, fp) != NULL) {
        cout << buffer << endl;
        send(sockfd, buffer, strlen(buffer), 0);
    }
    fclose(fp);
    close(sockfd);
    return 0;
}