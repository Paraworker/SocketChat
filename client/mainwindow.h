#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QCloseEvent>
#include<QTcpSocket>
#include"surfing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
        void closeEvent(QCloseEvent*event);


private slots:
    void after_connected();
    void after_disconnected();
    void read_text();
    void on_pushButton_send_clicked();
    void on_pushButton_connect_clicked();

private:
    Ui::MainWindow *ui;
    SocketInfo socinfo;

};
#endif // MAINWINDOW_H
