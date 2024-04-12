# Qt\_modbus\_TCP

使用Qt中的QModbusTcpClient模块进行IP/TCP通讯，实现对电路板某寄存器地址的值进行修改和读取。

## 项目背景

某公司提供了一个电路板，连接了一个遥控器，同时用一根网线连接本地计算机。遥控器上有一些按钮，可以操控电路板中不同寄存器的值。需要通过C++语言实现遥控器点击某按钮后，软件做出一定的响应，比如触发某快捷键。

## 版本

*   Qt5.9.8

*   C++11

## 功能

*   获取某地址寄存器的当前值，并基于其值的不同做出不同的响应；（连接了一个遥控器，可通过按钮修改其值）

*   修改某地址寄存器的当前值；（比如，将其值重置为0）

## 使用方法

直接运行pro文件即可，无需添加任何外部库。要注意路径中不能存在中文。
