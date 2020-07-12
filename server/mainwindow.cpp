#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpServer>
#include <QTcpSocket>
#include<QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serverSocket = nullptr;
    clientSocket[0] = nullptr;
    clientSocket[1] = nullptr;
}

MainWindow::~MainWindow()
{
    delete serverSocket;
    delete clientSocket[0];
    delete clientSocket[1];
    delete ui;
}


void MainWindow::on_Button_start_clicked()
{
    serverSocket = new QTcpServer(this);
    serverSocket->listen(QHostAddress::Any, 18103);
    connect(serverSocket,&QTcpServer::newConnection,this,&MainWindow::after_newConnection);
    this->ui->listWidget->addItem("Listening...");
    this->ui->Button_start->setDisabled(true);
    this->ui->Button_start->setText("已启动");
}

void MainWindow::after_newConnection(){
    int tag,ftag;
    if(clientSocket[0] == nullptr){
        tag = 0;
        ftag = 1;
    }
    else if(clientSocket[1] == nullptr){
        tag = 1;
        ftag =0;
    }
    else{
        this->ui->listWidget->addItem("有新的客户端连接请求，但是连接的客户端数量已满");
        return;
    }
    clientSocket[tag] = serverSocket->nextPendingConnection();
    QString ip = clientSocket[tag]->peerAddress().toString();
    unsigned int port = clientSocket[tag]->peerPort();
    connect(clientSocket[tag],&QTcpSocket::readyRead,this,[=](){
        forward(tag);
    });
    connect(clientSocket[tag],&QTcpSocket::disconnected,this,[=](){
        if(clientSocket[ftag] != nullptr){
            clientSocket[ftag]->write("1#对方下线了");
        }
        clientSocket[tag] = nullptr;
        this->ui->listWidget->addItem("Client " + QString::number(tag+1) + " disconnected");
    });
    if(clientSocket[ftag] == nullptr){
        clientSocket[tag]->write("1#对方不在线");
    }
    else{
        clientSocket[ftag]->write("1#对方上线了");
        clientSocket[tag]->write("1#对方在线");
    }
    this->ui->listWidget->addItem("Client " + QString::number(tag+1) + " connected    " + QString("[%1]:%2").arg(ip).arg(port));
}

void MainWindow::forward(int i){
    int ftag;
    if(i == 0){
        ftag = 1;
    }
    else{
        ftag = 0;
    }
    QByteArray array = clientSocket[i]->readAll();
    QString state;
    if(NULL != clientSocket[ftag]){
        clientSocket[ftag]->write(array.data());
        state = "(Forwarded)";
    }
    else{
        state = "(Not forwarded)";
    }
        this->ui->listWidget->addItem("From client " + QString::number(i+1) + " " + state + ": " + array.mid(2));
}

void MainWindow::closeEvent(QCloseEvent*event){
    close_socket();
    event->accept();
}

void MainWindow::close_socket(){
    for(int i = 0;i<2;i++){
        if(clientSocket[i] != nullptr){
            clientSocket[i]->disconnectFromHost();
            clientSocket[i]->close();
        }
    }
}

