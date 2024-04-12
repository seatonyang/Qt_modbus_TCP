#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QModbusTcpClient>
#include <QTimer>


#pragma execution_character_set("utf-8")


// 定义相关常量
const QString MODBUS_SERVER_IP = "192.168.0.150"; // Modbus TCP服务器的地址
const int MODBUS_SERVER_PORT = 502;               // Modbus TCP服务器的端口
const quint16 REGISTER_ADDRESS = 0x0A;            // 要读取的寄存器地址

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    QTimer *m_timerClient = nullptr;

    QModbusTcpClient *m_client = nullptr;

    bool connect2modbus(QString ipAddress,int port);

    void resetHoldingRegisters();



signals:

    void sigStateON();

    void sigStateOFF();

    void sigSwitchCamera();

    void sigMeasure();

private slots:

    void onWritingFinished();

    void onReadingFinished();

    void onTimeOut();

    void onStateChanged();


};

#endif // MAINWINDOW_H
