#include "serialport.h"
#include "helper_p.h"
#include "serialport_p.h"
#include "serialportinfo.h"
#include <QDebug>
#include <QElapsedTimer>

SerialPort::OpenMode convertMode(sp_mode mode);
sp_mode convertMode(SerialPort::OpenMode mode);
SerialPort::Parity convertParity(sp_parity parity);
sp_parity convertParity(SerialPort::Parity parity);
SerialPort::FlowControl convertFlowControl(sp_flowcontrol flow);
sp_flowcontrol convertFlowControl(SerialPort::FlowControl flow);
SerialPort::StopBits convertStopBits(int stopbits);
SerialPort::BaudRate convertBaudRate(int baudrate);

SerialPort::SerialPort(QObject* parent)
    : QIODevice(parent)
{
    qDebug(__PRETTY_FUNCTION__);
}

SerialPort::SerialPort(const QString& name, QObject* parent)
    : QIODevice(parent)
{
    qDebug(__PRETTY_FUNCTION__);
    if (setPortName(name))
        qDebug("Port setup");
}

SerialPort::SerialPort(const SerialPortInfo& info, QObject* parent)
    : QIODevice(parent)
{
    qDebug(__PRETTY_FUNCTION__);
    setPortName(info.portName());
}
SerialPort::~SerialPort()
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    if (isOpen())
        close();
    sp_free_port(d->port);
}

bool SerialPort::open(QIODevice::OpenMode mode)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    sp_return error;

    error = sp_open(d->port, convertMode(mode));
    setOpenMode(mode);
    return !helper::check(error) && QIODevice::open(mode);
}

void SerialPort::close()
{

    qDebug(__PRETTY_FUNCTION__);
    emit aboutToClose();
    Q_D(const SerialPort);
    sp_return error;
    error = sp_close(d->port);
    helper::check(error);
}

qint64 SerialPort::pos() const
{
    qDebug(__PRETTY_FUNCTION__);
    // Sequential devices doesn't provide this func
    return 0;
}

qint64 SerialPort::size() const
{
    qDebug(__PRETTY_FUNCTION__);
    // return buffer.size() + QIODevice::bytesAvailable();
    return QIODevice::bytesAvailable();
}

bool SerialPort::atEnd() const
{
    qDebug(__PRETTY_FUNCTION__);
    return QIODevice::atEnd();
}
/*!
    \reimp

    Returns the number of incoming bytes that are waiting to be read.

    \sa bytesToWrite(), read()
*/
qint64 SerialPort::bytesAvailable() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    sp_return error;
    error = sp_input_waiting(d->port);
    uint result;
    result = helper::check(error);
    return QIODevice::bytesAvailable() + result;
}

qint64 SerialPort::bytesToWrite() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    sp_return error;
    error = sp_output_waiting(d->port);
    uint result;
    result = helper::check(error);
    return QIODevice::bytesToWrite() + result;
}

bool SerialPort::waitForReadyRead(int msecs)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(SerialPort);
    return d->waitForReadyRead(msecs);
}

bool SerialPort::waitForBytesWritten(int msecs)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    Q_UNUSED(msecs)
    sp_return error;
    error = sp_drain(d->port);
    helper::check(error);
    return false;
}
/*!
    \reimp

    Always returns \c true. The serial port is a sequential device.
*/
bool SerialPort::isSequential() const
{
    qDebug(__PRETTY_FUNCTION__);
    return true;
}

//QByteArray SerialPort::read(qint64 maxlen)
//{

//    qDebug(__PRETTY_FUNCTION__);

//    return QIODevice::read(maxlen);
//}

//    int result_len;
//    QScopedArrayPointer<char> buffer(new char[SERIALPORT_BUFFERSIZE]);
//    result_len = readData(buffer.get(), maxlen);
//    qDebug() << result_len;
//    char* bytebuf = new char[result_len];
//    memcpy(bytebuf, buffer.get(), result_len);
//    return QByteArray::fromRawData(bytebuf, result_len);

//qint64 SerialPort::write(const char* data, qint64 len)
//{
//    Q_D(const SerialPort);
//    sp_return error;
//    QDebug dbg = qDebug();
//    for (int i = 0; i < len; ++i) {
//        dbg << QString::number(data[i], 16);
//    }
//    error = sp_blocking_write(d->port, data, len, m_timeout);
//    return helper::check(error);
//}

//qint64 SerialPort::write(const char* data)
//{
//    size_t len = strlen(data);
//    return write(data, len);
//}

QString SerialPort::portName() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    return sp_get_port_name(d->port);
}

bool SerialPort::setPortName(const QString& name)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(SerialPort);
    sp_return error;
    error = sp_get_port_by_name(name.toStdString().c_str(), &d->port);
    return !helper::check(error);
}

qint64 SerialPort::readData(char* data, qint64 maxlen)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    sp_return error;
    error = sp_blocking_read(d->port, data, maxlen, m_timeout);
    uint result;
    result = helper::check(error);
    return result;
}

qint64 SerialPort::writeData(const char* data, qint64 len)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    sp_return error;
    error = sp_blocking_write(d->port, data, len, m_timeout);
    uint result;
    result = helper::check(error);
    return result;
}

bool SerialPort::canReadLine() const
{
    return false;
}

SerialPort::FlowControl SerialPort::flowControl() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    return d->flowControl;
}

bool SerialPort::setFlowControl(const FlowControl& flowControl)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(SerialPort);
    if (isOpen())
        if (d->flowControl != flowControl) {
            sp_return error;
            error = sp_set_flowcontrol(d->port, convertFlowControl(flowControl));
            helper::check(error);
            d->flowControl = flowControl;
            emit flowControlChanged(d->flowControl);
            return true;
        }
    return false;
}

SerialPort::StopBits SerialPort::stopBits() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    return d->stopBits;
}

bool SerialPort::setStopBits(const StopBits& stopBits)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(SerialPort);
    if (isOpen())
        if (d->stopBits != stopBits) {
            sp_return error;
            error = sp_set_stopbits(d->port, stopBits);
            helper::check(error);
            d->stopBits = stopBits;
            emit stopBitsChanged(d->stopBits);
            return true;
        }
    return false;
}

SerialPort::Parity SerialPort::parity() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    return d->parity;
}

bool SerialPort::setParity(const Parity& parity)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(SerialPort);
    if (isOpen())
        if (d->parity != parity) {
            sp_return error;
            error = sp_set_parity(d->port, convertParity(parity));
            helper::check(error);
            d->parity = parity;
            emit parityChanged(d->parity);
            return true;
        }
    return false;
}

SerialPort::DataBits SerialPort::dataBits() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    return d->dataBits;
}

bool SerialPort::setDataBits(const DataBits& dataBits)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(SerialPort);
    if (isOpen())
        if (d->dataBits != dataBits) {
            sp_return error;
            error = sp_set_bits(d->port, dataBits);
            helper::check(error);
            d->dataBits = dataBits;
            emit dataBitsChanged(d->dataBits);
            return true;
        }
    return false;
}

int SerialPort::baudRate() const
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(const SerialPort);
    return d->baudRate;
}

bool SerialPort::setBaudRate(const int& baudRate)
{
    qDebug(__PRETTY_FUNCTION__);
    Q_D(SerialPort);
    if (isOpen())
        if (d->baudRate != baudRate) {
            sp_return error;
            error = sp_set_baudrate(d->port, baudRate);
            helper::check(error);
            d->baudRate = baudRate;
            emit baudRateChanged(d->baudRate);
            return true;
        }
    return false;
}

QIODevice::OpenMode convertMode(sp_mode mode)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (mode) {
    case SP_MODE_READ:
        return QIODevice::ReadOnly;
    case SP_MODE_WRITE:
        return QIODevice::WriteOnly;
    case SP_MODE_READ_WRITE:
        return QIODevice::ReadWrite;
    default:
        return QIODevice::NotOpen;
    }
}

sp_mode convertMode(QIODevice::OpenMode mode)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (mode) {
    case QIODevice::ReadOnly:
        return SP_MODE_READ;
    case QIODevice::WriteOnly:
        return SP_MODE_WRITE;
    case QIODevice::ReadWrite:
        return SP_MODE_READ_WRITE;
    default:
        abort();
    }
}

SerialPort::Parity convertParity(sp_parity parity)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (parity) {
    case SP_PARITY_NONE:
        return SerialPort::Parity::NoParity;
    case SP_PARITY_ODD:
        return SerialPort::Parity::OddParity;
    case SP_PARITY_EVEN:
        return SerialPort::Parity::EvenParity;
    case SP_PARITY_MARK:
        return SerialPort::Parity::MarkParity;
    case SP_PARITY_SPACE:
        return SerialPort::Parity::SpaceParity;
    case SP_PARITY_INVALID:
        return SerialPort::Parity::UnknownParity;
    default:
        abort();
    }
}

sp_parity convertParity(SerialPort::Parity parity)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (parity) {
    case SerialPort::Parity::NoParity:
        return SP_PARITY_NONE;
    case SerialPort::Parity::OddParity:
        return SP_PARITY_ODD;
    case SerialPort::Parity::EvenParity:
        return SP_PARITY_EVEN;
    case SerialPort::Parity::MarkParity:
        return SP_PARITY_MARK;
    case SerialPort::Parity::SpaceParity:
        return SP_PARITY_SPACE;
    case SerialPort::Parity::UnknownParity:
        return SP_PARITY_INVALID;
    default:
        abort();
    }
}

SerialPort::FlowControl convertFlowControl(sp_flowcontrol flow)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (flow) {
    case SP_FLOWCONTROL_NONE:
        return SerialPort::FlowControl::NoFlowControl;
    case SP_FLOWCONTROL_XONXOFF:
        return SerialPort::FlowControl::SoftwareControl;
    case SP_FLOWCONTROL_RTSCTS:
        return SerialPort::FlowControl::RTSCTSControl;
    case SP_FLOWCONTROL_DTRDSR:
        return SerialPort::FlowControl::DTRDSRControl;
    default:
        abort();
    }
}

sp_flowcontrol convertFlowControl(SerialPort::FlowControl flow)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (flow) {
    case SerialPort::FlowControl::NoFlowControl:
        return SP_FLOWCONTROL_NONE;
    case SerialPort::FlowControl::SoftwareControl:
        return SP_FLOWCONTROL_XONXOFF;
    case SerialPort::FlowControl::RTSCTSControl:
        return SP_FLOWCONTROL_RTSCTS;
    case SerialPort::FlowControl::DTRDSRControl:
        return SP_FLOWCONTROL_DTRDSR;
    default:
        abort();
    }
}

SerialPort::StopBits convertStopBits(int stopbits)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (stopbits) {
    case 1:
        return SerialPort::OneStop;
    case 2:
        return SerialPort::TwoStop;
    case 3:
        return SerialPort::OneAndHalfStop;
    default:
        abort();
    }
}

SerialPort::BaudRate convertBaudRate(int baudrate)
{
    qDebug(__PRETTY_FUNCTION__);
    switch (baudrate) {
    case 1200:
        return SerialPort::Baud1200;
    case 2400:
        return SerialPort::Baud2400;
    case 4800:
        return SerialPort::Baud4800;
    case 9600:
        return SerialPort::Baud9600;
    case 19200:
        return SerialPort::Baud19200;
    case 38400:
        return SerialPort::Baud38400;
    case 57600:
        return SerialPort::Baud57600;
    case 115200:
        return SerialPort::Baud115200;
    default:
        return SerialPort::Baud9600;
    }
}

bool SerialPortPrivate::waitForReadyRead(int msecs)
{
    //Q_Q(SerialPort);
    qDebug(__PRETTY_FUNCTION__);
    sp_return error;
    struct sp_event_set* event_set;

    error = sp_new_event_set(&event_set);
    helper::check(error);

    error = sp_add_port_events(event_set, port, SP_EVENT_RX_READY);
    helper::check(error);

    error = sp_wait(event_set, msecs);
    helper::check(error);
    sp_free_event_set(event_set);

    error = sp_input_waiting(port);
    //emit q->readyRead();
    return !helper::check(error);
}

bool SerialPortPrivate::waitForBytesWritten(int msecs)
{
    qDebug(__PRETTY_FUNCTION__);
    sp_return error;
    struct sp_event_set* event_set;

    error = sp_new_event_set(&event_set);
    helper::check(error);

    error = sp_add_port_events(event_set, port, SP_EVENT_TX_READY);
    helper::check(error);

    error = sp_wait(event_set, msecs);
    helper::check(error);
    sp_free_event_set(event_set);

    error = sp_output_waiting(port);
    return !helper::check(error);
}
