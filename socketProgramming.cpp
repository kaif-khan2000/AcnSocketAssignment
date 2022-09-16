
//server program

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
    servaddr.sin_port = htons(8080);
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
    servaddr.sin_port = htons(8080);
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        cout << "Error in connecting1" << endl;
        exit(0);
    }
    cout << "Socket connected" << endl;
    
    return sockfd;
}


void *read(void *arg){
    int sock = *((int *)arg);
    while(true){
        char buffer[1024];
        int n = read(sock, buffer, 1024);
        if (n < 0) {
            cout << "Error in reading" << endl;
            exit(0);
        }
        cout << "\b\bMessage: " << buffer << endl;
        cout << ">>";
    }
}

void *write(void *arg){
    int sock = *((int *)arg);
    while(true){
        char buffer[1024];
        cout << ">>";
        cin.getline(buffer, 1024);
        int n = write(sock, buffer, strlen(buffer));
        if (n < 0) {
            cout << "Error in writing" << endl;
            exit(0);
        }
    }
}