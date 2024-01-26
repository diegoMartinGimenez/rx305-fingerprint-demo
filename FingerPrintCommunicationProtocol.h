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
    virtual void search() = 0;

signals:
    void commandSuccess();
    void commandError(QString error);
    void fingerprintFound(int id);


};

#endif // FINGERPRINTCOMMUNICATIONPROTOCOL_H
