#include "commsa.h"

CommSA::CommSA()
{
    this->type = "text/plain";
    this->port = 80;
    this->method = "GET";
    this->timeOut = 1000;
}

CommSA::~CommSA()
{

}

std::string CommSA::sendReq(const char* parameters)
{
    return this->sendReq(this->host, this->route, this->method, this->type, this->port, parameters);
}

std::string CommSA::sendReq(const char* host, const char* route, const char* method, const char* type, int port, const char* parameters)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
//        cout << "WSAStartup failed.\n";
        return "";
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    struct hostent *shost;
    shost = gethostbyname(host);
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(port);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)shost->h_addr);

    // Ajusta o timeout para a conexão
    setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&this->timeOut, sizeof(this->timeOut));
    setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&this->timeOut, sizeof(this->timeOut));

    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
        printf("Could not connect");
        return "";
    }

    std::string request;
    request.append(method);
    request.append(" ");
    request.append(route);
    request.append(" HTTP/1.0\n");
    request.append("Host: ");
    request.append(host);
    request.append("\nContent-Type: ");
    request.append(type);
    request.append("; charset=utf-8\n\n\n");
    request.append(parameters);

//    std::cout << request.c_str() << std::endl;

    send(Socket, request.c_str(), strlen(request.c_str()),0);
    char buff[10000];
    int nDataLength;
    while ((nDataLength = recv(Socket,buff,10000,0)) > 0){
        int i = 0;
        while (buff[i] >= 32 || buff[i] == '\n' || buff[i] == '\r') {
//            std::cout << buff[i];
            i += 1;
        }
    }

    closesocket(Socket);
    WSACleanup();
    return buff;
}
const char *CommSA::getHost() const
{
    return host;
}

void CommSA::setHost(const char *value)
{
    host = value;
}
int CommSA::getPort() const
{
    return port;
}

void CommSA::setPort(int value)
{
    port = value;
}
const char *CommSA::getMethod() const
{
    return method;
}

void CommSA::setMethod(const char *value)
{
    method = value;
}
const char *CommSA::getType() const
{
    return type;
}

void CommSA::setType(const char *value)
{
    type = value;
}
const char *CommSA::getRoute() const
{
    return route;
}

void CommSA::setRoute(const char *value)
{
    route = value;
}
int CommSA::getTimeOut() const
{
    return timeOut;
}

void CommSA::setTimeOut(int value)
{
    timeOut = value;
}






