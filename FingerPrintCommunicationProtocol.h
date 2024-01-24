#ifndef FINGERPRINTCOMMUNICATIONPROTOCOL_H
#define FINGERPRINTCOMMUNICATIONPROTOCOL_H
#include <QObject>
// TODO - Renombrar la clase a FingerprintDevice
class FingerPrintCommunicationProtocol: public QObject
{        
    Q_OBJECT
public:
    virtual bool open() = 0;
    virtual void cancel() = 0;
    virtual bool enroll(int id) = 0;
signals:
    void commandSuccess();


};

#endif // FINGERPRINTCOMMUNICATIONPROTOCOL_H
