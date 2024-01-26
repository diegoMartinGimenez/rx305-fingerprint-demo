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

    m_fingerprint = new FP_GrowRX305();
    QObject::connect(this->m_fingerprint, &FingerPrintCommunicationProtocol::commandSuccess, this, &FingerPrintMgr::commandSuccess);
    QObject::connect(this->m_fingerprint, &FingerPrintCommunicationProtocol::commandError, this, &FingerPrintMgr::commandError);
    QObject::connect(this->m_fingerprint, &FingerPrintCommunicationProtocol::fingerprintFound, this, &FingerPrintMgr::fingerprintFound);

}

bool FingerPrintMgr::openFingerPrintPort()
{
    return m_fingerprint->open();

}

void FingerPrintMgr::enroll(int id)
{
    m_fingerprint->enroll(id);
}

void FingerPrintMgr::cancel()
{
    m_fingerprint->cancel();
}

void FingerPrintMgr::search()
{
    m_fingerprint->search();
}
