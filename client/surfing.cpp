#include"surfing.h"
#include<QTcpSocket>


void close_connect(SocketInfo& s){
    s.clientSocket->disconnectFromHost();
    s.clientSocket->close();
}




