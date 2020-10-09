#include "libserialport.h"
#include "serialport.h"
#include "serialportinfo.h"
#include <QCoreApplication>
#include <QDebug>
#include <QString>

int check(sp_return result);

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    QList<SerialPortInfo> aval_ports = SerialPortInfo::availablePorts();
    qDebug() << aval_ports;
    SerialPort port1("COM4");
    qDebug("Port created");
    port1.open(QIODevice::ReadWrite);
    qDebug("Port opened");
    port1.setBaudRate(9600);
    port1.setDataBits(SerialPort::Data8);
    port1.setParity(SerialPort::NoParity);
    port1.setStopBits(SerialPort::OneStop);
    port1.setFlowControl(SerialPort::NoFlowControl);
    QByteArray array; //(0x01, 0x04, 0x00, 0x01, 0x00, 0x1e, 0x21, 0xc2, '\0');
    array = QByteArrayLiteral("\x01\x04\x00\x01\x00\x1e\x21\xC2\x00");
    port1.write(array);
    port1.waitForBytesWritten();
    QByteArray ba1;
    ba1 = port1.read(128);

    qDebug() << ba1;
    port1.close();
    ba1.clear();
    qDebug("Port closed");
    return a.exec();
}

/* Helper function for error handling. */
int check(sp_return result)
{
    /* For this example we'll just exit on any error by calling abort(). */
    char* error_message;

    switch (result) {
    case SP_ERR_ARG:
        error_message = sp_last_error_message();
        qDebug("Error: Invalid argument: %s", error_message);
        sp_free_error_message(error_message);
        abort();
    case SP_ERR_FAIL:
        error_message = sp_last_error_message();
        qDebug("Error: Failed: %s", error_message);
        sp_free_error_message(error_message);
        abort();
    case SP_ERR_SUPP:
        qDebug("Error: Not supported.");
        abort();
    case SP_ERR_MEM:
        qDebug("Error: Couldn't allocate memory.");
        abort();
    case SP_OK:
    default:
        return result;
    }
}
