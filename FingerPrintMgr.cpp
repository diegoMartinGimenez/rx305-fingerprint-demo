#include "FingerPrintMgr.h"
#include "FP_GrowRX305.h"

FingerPrintMgr* FingerPrintMgr::m_instance = nullptr;

FingerPrintMgr::FingerPrintMgr(QObject *parent) : QObject(parent)
{
    inicialize();
}


FingerPrintMgr *FingerPrintMgr::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new FingerPrintMgr();
    }
    return m_instance;
}

void FingerPrintMgr::inicialize()
{
    // TODO - Abstract factory - Cargar tipo de device segun configuracion

//    m_fingerprint.reset(new FP_GrowRX305());
    m_fingerprint = new FP_GrowRX305();
    QObject::connect(this->m_fingerprint, &FingerPrintCommunicationProtocol::commandSuccess, this, &FingerPrintMgr::commandSuccess);
}

bool FingerPrintMgr::openFingerPrintPort()
{
    return m_fingerprint->open();

}

void FingerPrintMgr::loop()
{
    m_fingerprint->enroll(1);


}

void FingerPrintMgr::cancel()
{
    m_fingerprint->cancel();
}
