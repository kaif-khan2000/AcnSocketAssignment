#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include "datatypes.cpp"

#define SIZE 1024
using namespace std;

Server serverList[10];
int serverCount = 0;

void handleMessage(int sock, sockaddr_in client,socklen_t len, char buf[]){
    cout << "done1" << endl;
    Message msg = Message(buf);
    cout << "Message received: " << msg.msg << "message Type: " << msg.msgType << endl;
    switch(msg.msgType){
        case 1:
            // register the server
            serverList[serverCount++] = Server(msg.msg);
            cout << "Server registered: " << msg.msg << endl;
            char buf[SIZE];
            sprintf(buf,"1");
            sendto(sock, buf, SIZE, 0, (struct sockaddr *)&client, len);
            break;

        case 2:
            // send all servers infromation to client
            char buf2[SIZE];
            sprintf(buf2,"%d",serverCount);
            sendto(sock, buf2, strlen(buf2), 0, (struct sockaddr *)&client, len);    
            for(int i=0; i<serverCount; i++){
                Message msg = Message(2, serverList[i].toString());
                char buffer[SIZE];
                strcpy(buffer, msg.toString().c_str());
                sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client, len);    
            }
            break;
        default:
            //invalid message
            cout << "Invalid message type" << endl;
    }
}

int main() {
    //create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
        cerr << "socket error" << endl;
        return 1;
    }
    //bind the socket to an IP address and port
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(1234);
    local.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sock, (sockaddr *)&local, sizeof(local)) < 0)
    {
        cerr << "bind error" << endl;
        return 2;
    }
    cout << "Bind success" << endl;

    // now receive messages from clients or servers and handle them accordingly.
    while(true){
        char buf[SIZE];
        sockaddr_in client;
        socklen_t len = sizeof(client);
        int n = recvfrom(sock, buf, SIZE-1, 0, (sockaddr *)&client, &len);
        buf[n] = '\0';
        cout << "Message received: " << buf << endl;
        handleMessage(sock,client,len,buf);
    }
    return 0;
}