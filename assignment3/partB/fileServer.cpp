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
    
    //sending file max of 1024 at a time.
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
    //handle client thread
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    //this is going to run forever until there is an interupt
    while (1)
    {
        //accepting client
        int sockfd = accept(sock, (struct sockaddr *)&cliaddr, &clilen);
        if (sockfd < 0)
        {
            cout << "Error in accepting" << endl;
            exit(0);
        }

        //receiving access token and checking if it is valid
        char buffer[1024];
        recv(sockfd, buffer, 1024, 0);
        if (atoi(buffer) != accessToken)
        {
            cout << "Invalid access token" << endl;
            bzero(buffer, 1024);
            //sending status to client
            strcpy(buffer, "Token invalid");
            send(sockfd, buffer, strlen(buffer), 0);

            close(sockfd);
            continue;
        }
        bzero(buffer, 1024);
        //sending status to client
        strcpy(buffer, "Token valid");
        send(sockfd, buffer, strlen(buffer), 0);

        cout << "Socket accepted" << endl;
        
        //receiving file name
        char filename[1024];
        recv(sockfd, filename, 1024, 0);
        cout << "Filename received "<<filename << endl;

        //fetching file type from path and checking if it is valid
        char fileType[10];
        cout << strlen(filename) << endl;
        int flag = 0,count = 0;
        for(int i=0;i<strlen(filename);i++){
            if(flag == 1){
                fileType[count] = filename[i];
                count++;
            }
            else if(filename[i]=='.'){
                flag = 1;
            }
        }
        fileType[count] = '\0';
        cout << type << " " << fileType << endl;
        if (strcmp(fileType, type) != 0)
        {
            bzero(buffer, 1024);
            cout << "Invalid file type" << endl;
            //sending status to client
            strcpy(buffer, "Invalid file type");
            send(sockfd, buffer, strlen(buffer), 0);
            close(sockfd);
            continue;
        }
        bzero(buffer, 1024);
        //sending status to client
        strcpy(buffer, "Valid file type");
        send(sockfd, buffer, strlen(buffer), 0);
        
        //opening file and checking if its present
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            //if not present then send error message
            char status[1024] = "File not found";
            cout << "Error in opening file " << filename << endl;
            send(sockfd, status, 1024, 0);
            close(sockfd);
            continue;
        }

        //if file is present then send filesize
        struct stat file_stats;
        stat(filename, &file_stats);
        char status[1024];
        sprintf(status,"%d",(int)file_stats.st_size);
        send(sockfd,status,1024,0);

        //send file
        send_file(fp, sockfd);
        
        fclose(fp);
    }
}
// creating a file server
int main()
{
    // reading fileserver information from the console
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

    // creating a socket
    int sock = createASocket(port);
    char ip[30];
    sprintf(ip,"127.0.0.1");

    //creating a server datastructure and registering with the bootstrap server
    Server ser = Server(ip, port, servicename, type, accessToken);
    if(ser.registerServer()){
        cout<<"Server registered successfully"<<endl;
    } else {
        cout<<"Server registration failed"<<endl;
        close(sock);
        exit(0);
    }
    

    cout << "registered" << endl;

    //once the server is registered, it starts listening for the clients
    listen(sock, 7);
    cout << "Socket listening" << endl;
    thread clients[5];

    //creating 5 threads to handle the clients
    for (int i = 0; i < 5; i++)
    {
        clients[i] = thread(handleClient, sock);
    }

    // joining the threads
    for (int i = 0; i < 5; i++)
    {
        clients[i].join();
    }
    return 0;
}