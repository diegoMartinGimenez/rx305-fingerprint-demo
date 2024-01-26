#ifndef FINGERPRINTMGR_H
#define FINGERPRINTMGR_H

#include <QObject>

#include "FingerPrintCommunicationProtocol.h"
//#include "FingerprintDevice.h"

class FingerPrintMgr : public QObject
{
    Q_OBJECT
public:
    static FingerPrintMgr* GetInstance();
    void inicialize();

    bool openFingerPrintPort();
    Q_INVOKABLE void enroll(int id);
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void search();

private:
    explicit FingerPrintMgr(QObject *parent = nullptr);

    static FingerPrintMgr* m_instance;
    FingerPrintCommunicationProtocol* m_fingerprint;

signals:
    void commandSuccess();
    void commandError(QString error);
    void fingerprintFound(int id);

};

#define FINGER_PRINT_MGR FingerPrintMgr::GetInstance()

#endif // FINGERPRINTMGR_H
