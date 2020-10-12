//#include "libserialport.h"
#include "serialport.h"
#include "serialportinfo.h"
#include <QCoreApplication>
#include <QDebug>
#include <QString>

#define QIODEVICE_DEBUG

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    QList<SerialPortInfo> aval_ports = SerialPortInfo::availablePorts();
    qDebug() << aval_ports;

    SerialPort port1("COM4", &a);
    qDebug("Port created");
    int error_code = port1.open(QIODevice::ReadWrite);
    if (error_code)
        qDebug("Port opened");
    if (port1.setBaudRate(9600))
        qDebug("Baud rate changed");
    if (port1.setDataBits(SerialPort::Data8))
        qDebug("Data bits changed");
    if (port1.setParity(SerialPort::NoParity))
        qDebug("Parity changed");
    if (port1.setStopBits(SerialPort::OneStop))
        qDebug("Stop bits changed");
    if (port1.setFlowControl(SerialPort::NoFlowControl))
        qDebug("Flow control changed");
    qDebug() << port1.openMode();
    QByteArray array; //(0x01, 0x04, 0x00, 0x01, 0x00, 0x1e, 0x21, 0xc2, '\0');
    array = QByteArrayLiteral("\x01\x04\x00\x01\x00\x1e\x21\xC2\x00");
    port1.write(array);
    //port1.waitForBytesWritten();
    QByteArray ba1;
    // ba1 = port1.read(128);
    //ba1 = port1.readAll();
    qDebug() << ba1;
    port1.close();
    ba1.clear();
    qDebug("Port closed");
    return a.exec();
}
