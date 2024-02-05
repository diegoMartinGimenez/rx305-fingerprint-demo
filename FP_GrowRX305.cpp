#include "FP_GrowRX305.h"

FP_GrowRX305::FP_GrowRX305()
{
    m_stateMachine = new FP_GrowRX305_StateMachine(this);

    QObject::connect(&m_port, &FP_SerialComunication::response, this, &FP_GrowRX305::processResponse);
    QObject::connect(&m_port, &FP_SerialComunication::timeout, this->m_stateMachine, &FP_GrowRX305_StateMachine::error);

    QObject::connect(this, &FP_GrowRX305::commandSuccess, this, &FingerPrintCommunicationProtocol::commandSuccess);
    QObject::connect(this, &FP_GrowRX305::commandError, this, &FingerPrintCommunicationProtocol::commandError);
    QObject::connect(this, &FP_GrowRX305::found, this, &FingerPrintCommunicationProtocol::fingerprintFound);

    QObject::connect(this, &FP_GrowRX305::enrolling, this->m_stateMachine, &FP_GrowRX305_StateMachine::enroll);
    QObject::connect(this, &FP_GrowRX305::commandSuccess, this->m_stateMachine, &FP_GrowRX305_StateMachine::success);
    QObject::connect(this, &FP_GrowRX305::commandError, this->m_stateMachine, &FP_GrowRX305_StateMachine::error);
    QObject::connect(this, &FP_GrowRX305::searching, this->m_stateMachine, &FP_GrowRX305_StateMachine::search);
    QObject::connect(this, &FP_GrowRX305::canceling, this->m_stateMachine, &FP_GrowRX305_StateMachine::cancel);
    QObject::connect(this, &FP_GrowRX305::deleteFp, this->m_stateMachine, &FP_GrowRX305_StateMachine::eraseFingerprint);

    QObject::connect(this->m_stateMachine, &FP_GrowRX305_StateMachine::sendCommand, this, &FP_GrowRX305::sendCommand);
    QObject::connect(this->m_stateMachine, &FP_GrowRX305_StateMachine::fingerprintFound, this, &FP_GrowRX305::sendFingerFound);

}

void FP_GrowRX305::cancel()
{
    emit canceling();
}

bool FP_GrowRX305::open()
{
    return true;
}

bool FP_GrowRX305::enroll(int id)
{
    emit enrolling(id);
    QThread::currentThread()->setObjectName("Main Thread");

    return true;
}

void FP_GrowRX305::search()
{
    emit searching();
}

void FP_GrowRX305::deleteFingerprint(int id)
{
    emit deleteFp(id);
}

void FP_GrowRX305::processResponse()
{
    QByteArray response = m_port.getResponse();
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

void FP_GrowRX305::sendCommand()
{
    qDebug() << this << Q_FUNC_INFO << QThread::currentThread();

    m_port.commandToSend(m_stateMachine->command());
    m_port.sendCommand();

}

void FP_GrowRX305::sendFingerFound()
{
    QByteArray response = m_port.getResponse();
    quint8 id[2];
    id[0] = static_cast<quint8>(response[10]);
    id[1] = static_cast<quint8>(response[11]);
    unsigned short decimalNumber = (id[0] << 8) | id[1];

    qDebug() << "Valor decimal:" << decimalNumber;
    emit found(decimalNumber);
}


