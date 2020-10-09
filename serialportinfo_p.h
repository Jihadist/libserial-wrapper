#pragma once
#include <QString>

class SerialPortInfoPrivate {
public:
    QString portName;
    QString device;
    QString description;
    QString manufacturer;
    QString serialNumber;

    quint16 vendorIdentifier = 0;
    quint16 productIdentifier = 0;

    bool hasVendorIdentifier = false;
    bool hasProductIdentifier = false;
};

class SerialPortInfoPrivateDeleter {
public:
    static void cleanup(SerialPortInfoPrivate* p)
    {
        delete p;
    }
};
