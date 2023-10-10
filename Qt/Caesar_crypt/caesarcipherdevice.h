#ifndef CAESARCIPHERDEVICE_H
#define CAESARCIPHERDEVICE_H

#include <QIODevice>

class CaesarCipherDevice : public QIODevice
{
    Q_OBJECT
    Q_PROPERTY(int key READ key WRITE setKey)
public:
    explicit CaesarCipherDevice(QObject* parent = nullptr)
        : QIODevice(parent) {
        m_key = 0;
        m_device = nullptr;
    }

    void setBaseDevice(QIODevice* dev) {m_device=dev;}
    QIODevice *baseDevice() const {return m_device;}
    void setKey(int k) {m_key = k;}
    int key() const {return m_key;}

    //reimplement

    bool open(OpenMode mode) {
        if(!baseDevice())
            return false;
        if(baseDevice()->openMode() != mode)
            return false;
        return QIODevice::open(mode);
    }

    qint64 writeData(const char *data, qint64 len) {
        QByteArray ba(data, len);
        for(int i=0;i<len;++i)
            ba.data()[i] += m_key;
        int written = m_device->write(ba);
        emit bytesWritten(written);
        return written;
    }

    qint64 readData(char *data, qint64 maxlen)
    {
        QByteArray baseData = m_device->read(maxlen);
        const int s = baseData.size();
        for(int i=0;i<s;++i)
            data[i] = baseData[i] - m_key;
        return s;
    }
    //
private:
    int m_key;
    QIODevice *m_device;
};

//reimplementation
/*
bool CaesarCipherDevice::open(OpenMode mode) {
    if(!baseDevice())
        return false;
    if(baseDevice()->openMode() != mode)
        return false;
    return QIODevice::open(mode);
}

qint64 CaesarCipherDevice::writeData(const char *data, qint64 len) {
    QByteArray ba(data, len);
    for(int i=0;i<len;++i)
        ba.data()[i] += m_key;
    int written = m_device->write(ba);
    emit bytesWritten(written);
    return written;
}

qint64 CaesarCipherDevice::readData(char *data, qint64 maxlen)
{
    QByteArray baseData = m_device->read(maxlen);
    const int s = baseData.size();
    for(int i=0;i<s;++i)
        data[i] = baseData[i] - m_key;
    return s;
}
*/

#endif // CAESARCIPHERDEVICE_H
