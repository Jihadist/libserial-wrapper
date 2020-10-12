#pragma once

#include "libserialport.h"
#include "serialport.h"
#ifndef SERIALPORT_BUFFERSIZE
#define SERIALPORT_BUFFERSIZE 32768
#endif
class SerialPortPrivate {
    //Q_DECLARE_PUBLIC(SerialPort)
    //inline SerialPort* q_func() { return static_cast<SerialPort*>(q_ptr); }
    //inline const SerialPort* q_func() const { return static_cast<const SerialPort*>(q_ptr); }
    //friend class Class;

public:
    SerialPortPrivate() {};

    sp_port* port;

    int baudRate;
    SerialPort::DataBits dataBits;
    SerialPort::Parity parity;
    SerialPort::StopBits stopBits;
    SerialPort::FlowControl flowControl;

    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);
};
