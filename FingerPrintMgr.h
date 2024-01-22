#ifndef FINGERPRINTMGR_H
#define FINGERPRINTMGR_H

#include <QObject>
#include <QScopedPointer>

#include "FingerPrintCommunicationProtocol.h"
//#include "FingerprintDevice.h"

class FingerPrintMgr : public QObject
{
    Q_OBJECT
public:
    static FingerPrintMgr* GetInstance();
    void inicialize();

    bool openFingerPrintPort();
    Q_INVOKABLE void loop();
    Q_INVOKABLE void cancel();

private:
    explicit FingerPrintMgr(QObject *parent = nullptr);

    static FingerPrintMgr* m_instance;
    QScopedPointer <FingerPrintCommunicationProtocol> m_fingerprint;
//    QScopedPointer <FingerprintDevice> m_fingerprintDevice;

signals:
    void commandSuccess();
    void commandError(QString error);

};

#define FINGER_PRINT_MGR FingerPrintMgr::GetInstance()

#endif // FINGERPRINTMGR_H
