#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SIZE 1024
using namespace std;
class Message
{
public:
    int msgType;

public:
    char msg[SIZE];

public:
    Message(int msgType, char *msg)
    {
        this->msgType = msgType;
        strcpy(this->msg, msg);
    }

public:
    Message(int msgType, string msg)
    {
        this->msgType = msgType;
        strcpy(this->msg, msg.c_str());
    }

public:
    Message(char buf[])
    {
        this->msgType = atoi(strtok(buf, "@"));
        strcpy(this->msg, strtok(NULL, "@"));
    }

public:
    string toString()
    {
        char *buf;
        sprintf(buf, "%d@%s", this->msgType, this->msg);
        string s(buf);
        return s;
    }
};

class Server
{
public:
    char ip[30];

public:
    int port;

public:
    char servicename[50];

public:
    char type[10];

public:
    int accessToken;

public:
    Server() {}

public:
    Server(char ip[], int port, char servicename[], char type[], int accessToken)
    {
        strcpy(this->ip, ip);
        this->port = port;
        strcpy(this->servicename, servicename);
        strcpy(this->type, type);
        this->accessToken = accessToken;
    }

public:
    Server(char buf[])
    {
        strcpy(this->ip, strtok(buf, " "));
        this->port = atoi(strtok(NULL, " "));
        strcpy(this->servicename, strtok(NULL, " "));
        strcpy(this->type, strtok(NULL, " "));
        this->accessToken = atoi(strtok(NULL, " "));
    }

public:
    int registerServer()
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
        char buf[1024];
        char str[1000];
        strcpy(str,this->toString().c_str());
        sprintf(buf, "%d@%s", 1, str);
        sendto(sock2, buf, strlen(buf), 0, (sockaddr *)&peer, len);
        bzero(buf,SIZE);
        recvfrom(sock2, buf, SIZE, 0, (sockaddr *)&peer, &len);
        if (strcmp(buf, "1") == 0)
        {
            cout << "Server registered successfully" << endl;
            return 1;
        }
        else
        {
            cout << "Server registration failed" << endl;
            return 0;
        }
    }

public:
    string toString()
    {
        char buf[1024],*buf2;
        sprintf(buf, "%s %d %s %s %d", this->ip, this->port, this->servicename, this->type, this->accessToken);
        string s(buf);
        return s;
    }
};