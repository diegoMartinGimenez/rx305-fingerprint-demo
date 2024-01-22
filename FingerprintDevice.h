#ifndef FINGERPRINTDEVICE_H
#define FINGERPRINTDEVICE_H

#include <QObject>

class FingerprintDevice : public QObject
{
    Q_OBJECT
public:
    explicit FingerprintDevice(QObject *parent = nullptr);

signals:

};

#endif // FINGERPRINTDEVICE_H
