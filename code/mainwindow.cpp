#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QModbusDataUnit>
#include <QModbusReply>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_client = new QModbusTcpClient(this);
    m_timerClient = new QTimer(this);

    connect(m_client, &QModbusClient::stateChanged, this, &MainWindow::onStateChanged);//连接状态发生改变时处理函数
    // 建立连接
    connect2modbus(MODBUS_SERVER_IP, MODBUS_SERVER_PORT);
    // 启动定时器
    m_client->setTimeout(200); // 设置超时时间为200毫秒
    connect(m_timerClient, &QTimer::timeout, this, &MainWindow::onTimeOut);
    m_timerClient->start(500); // 设置计时器间隔为500毫秒
}

MainWindow::~MainWindow()
{
    if (m_client) {
        m_client->disconnectDevice();
    }
    delete ui;
}
/********************************************
 2  * 函数名称：Connect_to_modbus(QString IP_address,int Port)
 3  * 功能：连接到modbus设备
 4  * 工作方式：
 5  * 参数：
 6         参数1：modbus设备的IP地址               QString 类型
 7         参数2：modbus设备的端口号(一般用502)     int 类型
 8  * 返回值：成功返回true，失败返回fasle。
 9  * 备注：
10  * 修改记录
11 *********************************************/
bool MainWindow::connect2modbus(QString ipAddress, int port)
{
    if(!m_client){
        return false;
    }

    if (m_client->state() != QModbusDevice::ConnectedState) {

        //配置modbus tcp的连接参数 IP + Port   modbus协议的端口号为502
        m_client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ipAddress);
        m_client->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);

        if (!m_client->connectDevice()) {
            qDebug()<< "连接modbus设备失败";
            return false;
        }
        else {
            qDebug()<< "成功连接到modbus设备";
            return true;
        }
    }
    else {
        m_client->disconnectDevice();
        return false;
    }
}

/********************************************
2  * 函数名称：onStateChanged()
3  * 功能：监听TCP连接的状态，若状态发生改变，发出对应的信号
4  * 工作方式：
5  * 参数：无参数
6  * 返回值：无返回值
7  * 备注：
8  * 修改记录:
9 *********************************************/
void MainWindow::onStateChanged()
{
    //连接状态改变时的槽函数
    if(m_client->state() == QModbusDevice::ConnectedState)
    {
        emit sigStateON();
    }
    else {
        emit sigStateOFF();
    }
}

void MainWindow::onReadingFinished()
{
    QModbusReply *reply = qobject_cast<QModbusReply*>(sender());
    if (!reply) return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit modbusDataUnit = reply->result();
        int registerStatus = modbusDataUnit.value(0);

        if(registerStatus == 1){
            qDebug()<<"切换相机";
            emit sigSwitchCamera();
        }
        if(registerStatus == 2){
            emit sigMeasure();
            qDebug()<<"测量";
        }

    } else {
        qDebug() << "Read error: " << reply->errorString();
    }

    reply->deleteLater();

}

void MainWindow::resetHoldingRegisters()
{
    // 从地址REGISTER_ADDRESS开始写入1个寄存器
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, REGISTER_ADDRESS, 1);
    writeUnit.setValue(REGISTER_ADDRESS, 0); // 设置地址REGISTER_ADDRESS的值为0
    QModbusReply *reply_init = m_client->sendWriteRequest(writeUnit, 1); // 1是设备的Modbus地址
    if (reply_init) {
        if (!reply_init->isFinished()) {
            connect(reply_init, &QModbusReply::finished, this, &MainWindow::onWritingFinished);
        } else {
            // 请求已经完成，处理结果
            delete reply_init;
        }
    } else {
        qDebug() << "Write error";
    }


}

void MainWindow::onTimeOut()
{
    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, REGISTER_ADDRESS, 1); // 从地址0开始读取10个寄存器
    QModbusReply *reply = m_client->sendReadRequest(readUnit, 1); // 1是设备的Modbus地址
    if (reply != nullptr) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, &MainWindow::onReadingFinished);
        } else {
            delete reply;
        }
    } else {
        qDebug() << "Read error";
    }

    // 当回复完成时，删除reply
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    // 把寄存器的值重新赋值为0
    resetHoldingRegisters();


}


void MainWindow::onWritingFinished()
{
    QModbusReply *reply = qobject_cast<QModbusReply*>(sender());
    if (!reply) return;

    if (reply->error() == QModbusDevice::NoError) {

        // 这里可以实现相应的写入寄存器后需要执行的动作
//        qDebug() << "Write successful";
    } else {
        qDebug() << "Write error: " << reply->errorString();
    }

    reply->deleteLater();
}
