#include "FP_GrowRX305.h"

FP_GrowRX305::FP_GrowRX305()
{
    m_port = new FP_SerialComunication(this);
    m_stateMachine = new FP_GrowRX305_StateMachine(this);

//    QObject::connect(m_thread, &QThread::started, m_worker, [=]()->void{m_worker->doWork(m_port);});
//    QObject::connect(m_worker, &FP_Worker::done, m_worker, &FP_Worker::deleteLater);
//    QObject::connect(m_worker, &FP_Worker::done, m_thread, &QThread::quit);
//    QObject::connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

//    m_thread->start();

    QObject::connect(this->m_port, &FP_SerialComunication::response, this, &FP_GrowRX305::processResponse);
    QObject::connect(this, &FP_GrowRX305::commandSuccess, this, &FingerPrintCommunicationProtocol::commandSuccess);
    QObject::connect(this, &FP_GrowRX305::enrolling, this->m_stateMachine, &FP_GrowRX305_StateMachine::signalOne);
    QObject::connect(this, &FP_GrowRX305::commandSuccess, this->m_stateMachine, &FP_GrowRX305_StateMachine::success);
    QObject::connect(this, &FP_GrowRX305::commandError, this->m_stateMachine, &FP_GrowRX305_StateMachine::error);
    QObject::connect(this->m_stateMachine, &FP_GrowRX305_StateMachine::sendCommand, this, &FP_GrowRX305::sendCommandToWorker);
}

void FP_GrowRX305::cancel()
{
    m_worker->cancel();
}

bool FP_GrowRX305::open()
{
    return m_port->open();
}

bool FP_GrowRX305::enroll(int id)
{
    emit enrolling();
    Q_UNUSED(id)
    QThread::currentThread()->setObjectName("Main Thread");



//    m_port->commandToSend(m_stateMachine->command());


//    auto *thread = new QThread();
//    thread->setObjectName("Secondary Thread");
//    m_worker = new FP_Worker();
//    m_worker->moveToThread(thread);

//    QObject::connect(thread, &QThread::started, m_worker, [=]()->void{m_worker->doWork(m_port);});
//    QObject::connect(m_worker, &FP_Worker::done, m_worker, &FP_Worker::deleteLater);
//    QObject::connect(m_worker, &FP_Worker::done, thread, &QThread::quit);
//    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

//    thread->start();

    return true;
}

void FP_GrowRX305::processResponse()
{
    QByteArray response = m_port->getResponse();
    quint8 confirmationCode = static_cast<quint8>(response[9]);
    cCodes cCode = static_cast<cCodes>(confirmationCode);
    if (cCode == cCodes::FP_RESP_OK){
        qDebug() << "OK" << cCodeToString(cCode);
        emit commandSuccess();
    } else {
        qDebug() << "ERROR" << cCodeToString(cCode);
        emit commandError(cCodeToString(cCode));
    }
}

QString FP_GrowRX305::cCodeToString(FP_GrowRX305::cCodes code)
{
    static QMap<cCodes, QString> descriptions = {
        {cCodes::FP_RESP_OK, "Command executed successfully"},
        {cCodes::FP_RESP_RECIEVEERR, "Packet receive error"},
        {cCodes::FP_RESP_NOFINGER, "No finger detected"},
        {cCodes::FP_RESP_ENROLLFAIL, "Failed to enroll the finger"},
        {cCodes::FP_RESP_OVERDISORDERFAIL, "Failed to generate character file due to over-disorderly fingerprint image"},
        {cCodes::FP_RESP_OVERWETFAIL, "Failed to generate character file due to over-wet fingerprint image"},
        {cCodes::FP_RESP_OVERDISORDERFAIL2, "Failed to generate character file due to over-disorderly fingerprint image"},
        {cCodes::FP_RESP_FEATUREFAIL, "Failed to generate character file due to over-wet fingerprint image"},
        {cCodes::FP_RESP_DONOTMATCH, "Fingers do not match"},
        {cCodes::FP_RESP_NOTFOUND, "No valid match found"},
        {cCodes::FP_RESP_ENROLLMISMATCH, "Failed to combine character files (two character files (images) are used to create a template)"},
        {cCodes::FP_RESP_BADLOCATION, "Addressing PageID is beyond the finger library"},
        {cCodes::FP_RESP_INVALIDTEMPLATE, "Error when reading template from library or the template is invalid"},
        {cCodes::FP_RESP_TEMPLATEUPLOADFAIL, "Error when uploading template"},
        {cCodes::FP_RESP_PACKETACCEPTFAIL, "Module can not accept more packets"},
        {cCodes::FP_RESP_IMAGEUPLOADFAIL, "Error when uploading image"},
        {cCodes::FP_RESP_TEMPLATEDELETEFAIL, "Error when deleting template"},
        {cCodes::FP_RESP_DBCLEARFAIL, "Failed to clear fingerprint library"},
        {cCodes::FP_RESP_WRONGPASSOWRD, "Wrong password"},
        {cCodes::FP_RESP_IMAGEGENERATEFAIL, "Fail to generate the image due to lackness of valid primary image"},
        {cCodes::FP_RESP_FLASHWRITEERR, "Error when writing flash"},
        {cCodes::FP_RESP_NODEFINITIONERR, "No definition error"},
        {cCodes::FP_RESP_INVALIDREG, "Invalid register number"},
        {cCodes::FP_RESP_INCORRECTCONFIG, "Incorrect configuration of register"},
        {cCodes::FP_RESP_WRONGNOTEPADPAGE, "Wrong notepad page number"},
        {cCodes::FP_RESP_COMPORTERR, "Failed to operate the communication port"},
    };

    return descriptions.value(code, "Unknown error");

}

void FP_GrowRX305::sendCommandToWorker()
{
    qDebug() << this << Q_FUNC_INFO << QThread::currentThread();

    m_port->commandToSend(m_stateMachine->command());

    m_thread = new QThread();
    m_thread->setObjectName("Secondary Thread");
    m_worker = new FP_Worker();
    m_worker->moveToThread(m_thread);

    QObject::connect(m_thread, &QThread::started, m_worker, [=]()->void{m_worker->doWork(m_port);});
    QObject::connect(m_worker, &FP_Worker::done, m_worker, &FP_Worker::deleteLater);
    QObject::connect(m_worker, &FP_Worker::done, m_thread, &QThread::quit);
    QObject::connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    m_thread->start();

}


