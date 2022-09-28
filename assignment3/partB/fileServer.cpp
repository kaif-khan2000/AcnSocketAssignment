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
#include "datatypes.cpp"

#define SIZE 1024
using namespace std;

int accessToken;
char type[10];

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

int createASocket(int port)
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
    servaddr.sin_port = htons(port);
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
        char buffer[1024];
        recv(sockfd, buffer, 1024, 0);
        if (atoi(buffer) != accessToken)
        {
            cout << "Invalid access token" << endl;
            close(sockfd);
            continue;
        }
        cout << "Socket accepted" << endl;
        char filename[1024];
        recv(sockfd, filename, 1024, 0);
        cout << "Filename received "<<filename << endl;
        char fileType[10];
        for(int i=strlen(filename)-1;i>=0;i++){
            if(filename[i]=='.'){
                strcpy(fileType,filename+i+1);
                break;
            }
        }
        if (strcmp(fileType, type) != 0)
        {
            cout << "Invalid file type" << endl;
            close(sockfd);
            continue;
        }
        
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
    int port;
    cout << "Enter port: ";
    cin >> port;
    
    char servicename[50];
    cout << "Enter service name: ";
    cin >> servicename;

 
    cout << "Enter type (mkv,pdf,jpeg,png etc.): ";
    cin >> type;

    
    cout << "Enter access token: ";
    cin >> accessToken;

    int sock = createASocket(port);
    char ip[30];
    sprintf(ip,"127.0.0.1");
    Server ser = Server(ip, port, servicename, type, accessToken);
    if(ser.registerServer()){
        cout<<"Server registered successfully"<<endl;
    } else {
        cout<<"Server registration failed"<<endl;
        close(sock);
        exit(0);
    }
    

    cout << "registered" << endl;
    listen(sock, 7);
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