#pragma once
#include <QScopedPointer>
#include <QString>

class SerialPort;
class SerialPortInfoPrivate;
class SerialPortInfoPrivateDeleter;

class SerialPortInfo {

    Q_DECLARE_PRIVATE(SerialPortInfo)
public:
    friend QDebug operator<<(QDebug dbg, const SerialPortInfo& info);
    SerialPortInfo();
    explicit SerialPortInfo(const SerialPort& port);
    explicit SerialPortInfo(const QString& name);
    SerialPortInfo(const SerialPortInfo& other);
    ~SerialPortInfo();

    SerialPortInfo& operator=(const SerialPortInfo& other);
    void swap(SerialPortInfo& other);

    //static QList<qint32> standardBaudRates();
    static QList<SerialPortInfo> availablePorts();

    bool isNull() const;
    QString portName() const;
    QString systemLocation() const;
    QString description() const;
    QString manufacturer() const;
    QString serialNumber() const;

    quint16 vendorIdentifier() const;
    quint16 productIdentifier() const;

private:
    QScopedPointer<SerialPortInfoPrivate, SerialPortInfoPrivateDeleter> d_ptr;
    SerialPortInfo(const SerialPortInfoPrivate& dd);
};

inline bool SerialPortInfo::isNull() const
{
    return !d_ptr;
}
