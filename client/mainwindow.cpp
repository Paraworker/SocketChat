#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "surfing.h"
#include<QTcpSocket>
#include<QDateTime>
#include<QCloseEvent>
#include<QHostAddress>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socinfo.clientSocket = new QTcpSocket(this);
    this->ui->pushButton_send->setDisabled(true);
    this->ui->listWidget->setSpacing(8);
    connect(socinfo.clientSocket,&QTcpSocket::connected,this,&MainWindow::after_connected);
    connect(socinfo.clientSocket,&QTcpSocket::readyRead,this,&MainWindow::read_text);
    connect(socinfo.clientSocket,&QTcpSocket::disconnected,this,&MainWindow::after_disconnected);
}

MainWindow::~MainWindow()
{

    delete ui;
    delete socinfo.clientSocket;
}

void MainWindow::on_pushButton_send_clicked()
{
    QString text = this->ui->textEdit->toPlainText();
    if(text != ""){
         QString flag_added = "2#" + text;
         QDateTime current_time =QDateTime::currentDateTime();
         QString current_time_s =current_time.toString("hh:mm:ss    ddd");
         socinfo.clientSocket->write(flag_added.toUtf8().data());
         this->ui->textEdit->clear();
         this->ui->listWidget->addItem(current_time_s + "\n我：" + text);
    }
}

void MainWindow::after_connected(){
    this->ui->pushButton_connect->setDisabled(true);
    this->ui->pushButton_connect->setText("已连接");
    this->ui->pushButton_send->setEnabled(true);
}

void MainWindow::read_text(){
    QByteArray array = socinfo.clientSocket->readAll();
    QString s = array;
    if(s.startsWith("1#")){
        this->ui->listWidget->addItem(s.mid(2));
    }
    else if(s.startsWith("2#")){
        QDateTime current_time =QDateTime::currentDateTime();
        QString current_time_s =current_time.toString("hh:mm:ss    ddd");
        this->ui->listWidget->addItem(current_time_s + "\n我的朋友：" + s.mid(2));
    }
}


void MainWindow::on_pushButton_connect_clicked()
{
    socinfo.clientSocket->connectToHost(QHostAddress(socinfo.server_ip),socinfo.server_port);
}

void MainWindow::closeEvent(QCloseEvent*event){
    disconnect(socinfo.clientSocket,&QTcpSocket::disconnected,this,&MainWindow::after_disconnected);
    close_connect(socinfo);
    event->accept();
}

void MainWindow::after_disconnected(){
    this->ui->pushButton_send->setDisabled(true);
    this->ui->pushButton_connect->setText("连接到服务器");
    this->ui->pushButton_connect->setEnabled(true);
    QMessageBox::information(this, tr("连接断开"), tr("与服务器的连接断开了"));
}
