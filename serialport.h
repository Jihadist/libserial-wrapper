#pragma once
#include <QIODevice>
class SerialPortInfo;
class SerialPortPrivate;

class SerialPort : public QIODevice {

    Q_OBJECT
    Q_DECLARE_PRIVATE(SerialPort)
public:
    explicit SerialPort(QObject* parent = nullptr);
    explicit SerialPort(const QString& name, QObject* parent = nullptr);
    explicit SerialPort(const SerialPortInfo& info, QObject* parent = nullptr);
    //~SerialPort();

    enum BaudRate {
        Baud1200 = 1200,
        Baud2400 = 2400,
        Baud4800 = 4800,
        Baud9600 = 9600,
        Baud19200 = 19200,
        Baud38400 = 38400,
        Baud57600 = 57600,
        Baud115200 = 115200,
    };
    Q_ENUM(BaudRate)

    enum DataBits {
        Data5 = 5,
        Data6 = 6,
        Data7 = 7,
        Data8 = 8,
    };
    Q_ENUM(DataBits)

    enum Parity {
        NoParity = 0,
        OddParity = 1,
        EvenParity = 2,
        MarkParity = 3,
        SpaceParity = 4,
        UnknownParity = -1,
    };
    Q_ENUM(Parity)

    enum StopBits {
        OneStop = 1,
        OneAndHalfStop = 3,
        TwoStop = 2,
    };
    Q_ENUM(StopBits)

    enum FlowControl {
        NoFlowControl,
        SoftwareControl,
        RTSCTSControl,
        DTRDSRControl,
    };
    Q_ENUM(FlowControl)

    enum PinoutSignal {
        NoSignal = 0x00,
        TransmittedDataSignal = 0x01,
        ReceivedDataSignal = 0x02,
        DataTerminalReadySignal = 0x04,
        DataCarrierDetectSignal = 0x08,
        DataSetReadySignal = 0x10,
        RingIndicatorSignal = 0x20,
        RequestToSendSignal = 0x40,
        ClearToSendSignal = 0x80,
        SecondaryTransmittedDataSignal = 0x100,
        SecondaryReceivedDataSignal = 0x200
    };
    Q_FLAG(PinoutSignal)
public:
    bool open(OpenMode mode) override;
    void close() override;
    qint64 pos() const override;
    qint64 size() const override;
    bool atEnd() const override;
    qint64 bytesAvailable() const override;
    qint64 bytesToWrite() const override;
    bool waitForReadyRead(int msecs = 0) override;
    bool waitForBytesWritten(int msecs = 0) override;
    bool isSequential() const override;

    QByteArray read(qint64 maxlen);

    qint64 write(const char* data, qint64 len);
    qint64 write(const char* data);
    inline qint64 write(const QByteArray& data)
    {
        return write(data.constData(), data.size());
    }

    QString portName() const;
    bool setPortName(const QString& name);

    int baudRate() const;
    bool setBaudRate(const int& baudRate);

    DataBits dataBits() const;
    bool setDataBits(const DataBits& dataBits);

    Parity parity() const;
    bool setParity(const Parity& parity);

    StopBits stopBits() const;
    bool setStopBits(const StopBits& stopBits);

    FlowControl flowControl() const;
    bool setFlowControl(const FlowControl& flowControl);

signals:
    void baudRateChanged(int baudRate);
    void dataBitsChanged(SerialPort::DataBits dataBits);
    void parityChanged(SerialPort::Parity parity);
    void stopBitsChanged(SerialPort::StopBits stopBits);
    void flowControlChanged(SerialPort::FlowControl flowControl);

protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

private:
    uint m_timeout = 1000;
    QByteArray m_readChunkBuffer;
    QByteArray m_writeChunkBuffer;
    bool m_communicationStarted = false;
    bool m_writeStarted = false;
    bool m_readStarted = false;
    qint64 m_writeBytesTransferred = 0;
    qint64 m_readBytesTransferred = 0;

    Q_DISABLE_COPY(SerialPort)
    // QIODevice interface
};
