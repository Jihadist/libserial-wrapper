#include "serialportinfo.h"
#include "helper_p.h"
#include "libserialport.h"
#include "serialport.h"
#include "serialportinfo_p.h"
#include <QDebug>
#include <QList>
SerialPortInfo::SerialPortInfo()
{
}

SerialPortInfo::SerialPortInfo(const SerialPort& port)
    : SerialPortInfo(port.portName())
{
}

SerialPortInfo::SerialPortInfo(const QString& name)
{
    const auto infos = SerialPortInfo::availablePorts();
    for (const SerialPortInfo& info : infos) {
        if (name == info.portName()) {
            *this = info;
            break;
        }
    }
}

SerialPortInfo::SerialPortInfo(const SerialPortInfo& other)
    : d_ptr(other.d_ptr ? new SerialPortInfoPrivate(*other.d_ptr) : nullptr)
{
}

SerialPortInfo::~SerialPortInfo()
{
}

SerialPortInfo& SerialPortInfo::operator=(const SerialPortInfo& other)
{
    SerialPortInfo(other).swap(*this);
    return *this;
}

void SerialPortInfo::swap(SerialPortInfo& other)
{
    d_ptr.swap(other.d_ptr);
}

//QList<qint32> SerialPortInfo::standardBaudRates()
//{
//}

QList<SerialPortInfo> SerialPortInfo::availablePorts()
{
    sp_port** port_list;
    sp_return error;
    /* Call sp_list_ports() to get the ports. The port_list
     * pointer will be updated to refer to the array created. */
    error = sp_list_ports(&port_list);
    helper::check(error);

    QList<SerialPortInfo> serialPortInfoList;
    for (int i = 0; port_list[i] != NULL; i++) {
        SerialPortInfoPrivate priv;
        sp_port* port = port_list[i];
        sp_transport transport = sp_get_port_transport(port);

        if (transport == SP_TRANSPORT_NATIVE) {
            /* This is a "native" port, usually directly connected
                 * to the system rather than some external interface. */
        } else if (transport == SP_TRANSPORT_USB) {
            /* This is a USB to serial converter of some kind. */
            priv.portName = sp_get_port_name(port);
            //priv.device = QSerialPortInfoPrivate::portNameToSystemLocation(portName);
            priv.description = sp_get_port_description(port);
            priv.manufacturer = sp_get_port_usb_manufacturer(port);

            //const QString instanceIdentifier = deviceInstanceIdentifier(deviceInfoData.DevInst);

            priv.serialNumber = sp_get_port_usb_serial(port);
            int usb_vid, usb_pid;
            error = sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);
            if (error != SP_ERR_SUPP) {
                helper::check(error);
                if (usb_vid) {
                    priv.vendorIdentifier = usb_vid;
                    priv.hasVendorIdentifier = true;
                }
                if (usb_pid) {
                    priv.productIdentifier = usb_pid;
                    priv.hasProductIdentifier = true;
                }
            }
        }
        serialPortInfoList.append(priv);
        sp_free_port(port);
    }

    return serialPortInfoList;
}

QString SerialPortInfo::portName() const
{
    Q_D(const SerialPortInfo);
    return !d ? QString() : d->portName;
}

QString SerialPortInfo::systemLocation() const
{
    Q_D(const SerialPortInfo);
    return !d ? QString() : d->device;
}

QString SerialPortInfo::description() const
{
    Q_D(const SerialPortInfo);
    return !d ? QString() : d->description;
}

QString SerialPortInfo::manufacturer() const
{
    Q_D(const SerialPortInfo);
    return !d ? QString() : d->manufacturer;
}

QString SerialPortInfo::serialNumber() const
{
    Q_D(const SerialPortInfo);
    return !d ? QString() : d->serialNumber;
}

quint16 SerialPortInfo::vendorIdentifier() const
{
    Q_D(const SerialPortInfo);
    return !d ? 0 : d->vendorIdentifier;
}

quint16 SerialPortInfo::productIdentifier() const
{
    Q_D(const SerialPortInfo);
    return !d ? 0 : d->productIdentifier;
}

SerialPortInfo::SerialPortInfo(const SerialPortInfoPrivate& dd)
    : d_ptr(new SerialPortInfoPrivate(dd))
{
}

QDebug operator<<(QDebug dbg, const SerialPortInfo& info)
{
    dbg << info.description() << info.portName() << info.productIdentifier() << info.vendorIdentifier();
    return dbg.maybeSpace();
}
