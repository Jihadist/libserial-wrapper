#pragma once

#include "libserialport.h"
#include "serialport.h"
#ifndef SERIALPORT_BUFFERSIZE
#define SERIALPORT_BUFFERSIZE 32768
#endif
class SerialPortPrivate {
public:
    SerialPortPrivate() = default;

    sp_port* port;

    int baudRate;
    SerialPort::DataBits dataBits;
    SerialPort::Parity parity;
    SerialPort::StopBits stopBits;
    SerialPort::FlowControl flowControl;

    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);
};
