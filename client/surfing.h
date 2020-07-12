#ifndef SURFING_H
#define SURFING_H
#include<QTcpSocket>

struct SocketInfo{
    QTcpSocket* clientSocket;
    QString server_ip;
    qint16 server_port;
    SocketInfo():server_ip("47.114.170.109"),server_port(18103){}
};

void close_connect(SocketInfo& s);


#endif // SURFING_H
