#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include<QCloseEvent>

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
    void on_Button_start_clicked();
    void after_newConnection();

private:
    Ui::MainWindow *ui;
    QTcpServer* serverSocket;
    QTcpSocket* clientSocket[2];
    void forward(int i);
    void close_socket();
};
#endif // MAINWINDOW_H
