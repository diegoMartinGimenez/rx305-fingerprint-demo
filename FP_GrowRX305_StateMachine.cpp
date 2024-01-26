#include "FP_GrowRX305_StateMachine.h"
#include "FP_GrowRX305.h"

FP_GrowRX305_StateMachine::FP_GrowRX305_StateMachine(QObject *parent) : QObject(parent)
{
    m_sm = new QStateMachine(this);

    QState *idle = new QState(m_sm);

    QState *enroll = new QState(m_sm);
    QState *gettingImage1 = new QState(enroll);
    QState *gettingImage2 = new QState(enroll);
    QState *generatingChar1 = new QState(enroll);
    QState *generatingChar2 = new QState(enroll);
    QState *registeringModel = new QState(enroll);
    QState *storing = new QState(enroll);
    QState *search = new QState(m_sm);
    QState *searchGettingImage = new QState(search);
    QState *searchGenChar = new QState(search);
    QState *searchFp = new QState(search);
    QState *fpFound = new QState(search);


    m_sm->setInitialState(idle);

    QObject::connect(this, &FP_GrowRX305_StateMachine::enroll, this, &FP_GrowRX305_StateMachine::setIdToEnroll);

    idle->addTransition(this, &FP_GrowRX305_StateMachine::enroll, enroll);
    enroll->setInitialState(gettingImage1);

    enroll->addTransition(this, &FP_GrowRX305_StateMachine::cancel, idle);
    search->addTransition(this, &FP_GrowRX305_StateMachine::cancel, idle);

    gettingImage1->addTransition(this, &FP_GrowRX305_StateMachine::success, generatingChar1);
    gettingImage1->addTransition(this, &FP_GrowRX305_StateMachine::error, gettingImage1);

    generatingChar1->addTransition(this, &FP_GrowRX305_StateMachine::success, gettingImage2);
    generatingChar1->addTransition(this, &FP_GrowRX305_StateMachine::error, gettingImage1);

    gettingImage2->addTransition(this, &FP_GrowRX305_StateMachine::success, generatingChar2);
    gettingImage2->addTransition(this, &FP_GrowRX305_StateMachine::error, gettingImage2);

    generatingChar2->addTransition(this, &FP_GrowRX305_StateMachine::success, registeringModel);
    generatingChar2->addTransition(this, &FP_GrowRX305_StateMachine::error, idle);

    registeringModel->addTransition(this, &FP_GrowRX305_StateMachine::success, storing);
    registeringModel->addTransition(this, &FP_GrowRX305_StateMachine::error, idle);

    storing->addTransition(this, &FP_GrowRX305_StateMachine::success, idle);

    QObject::connect(gettingImage1, &QState::entered, this, &FP_GrowRX305_StateMachine::getImgCmd);
    QObject::connect(generatingChar1, &QState::entered, this, &FP_GrowRX305_StateMachine::gencharCommand1);
    QObject::connect(gettingImage2, &QState::entered, this, &FP_GrowRX305_StateMachine::getImgCmd);
    QObject::connect(generatingChar2, &QState::entered, this, &FP_GrowRX305_StateMachine::gencharCommand2);
    QObject::connect(registeringModel, &QState::entered, this, &FP_GrowRX305_StateMachine::regModelCmd);
    QObject::connect(storing, &QState::entered, this, &FP_GrowRX305_StateMachine::storeCmd);

    idle->addTransition(this, &FP_GrowRX305_StateMachine::search, search);
    search->setInitialState(searchGettingImage);

    searchGettingImage->addTransition(this, &FP_GrowRX305_StateMachine::success, searchGenChar);
    searchGettingImage->addTransition(this, &FP_GrowRX305_StateMachine::error, searchGettingImage);

    searchGenChar->addTransition(this, &FP_GrowRX305_StateMachine::success, searchFp);
    searchGenChar->addTransition(this, &FP_GrowRX305_StateMachine::error, idle);

    searchFp->addTransition(this, &FP_GrowRX305_StateMachine::success, fpFound);
    searchFp->addTransition(this, &FP_GrowRX305_StateMachine::error, searchGettingImage);

    fpFound->addTransition(idle);

    QObject::connect(searchGettingImage, &QState::entered, this, &FP_GrowRX305_StateMachine::getImgCmd);
    QObject::connect(searchGenChar, &QState::entered, this, &FP_GrowRX305_StateMachine::gencharCommand1);
    QObject::connect(searchFp, &QState::entered, this, &FP_GrowRX305_StateMachine::searchCmd);
    QObject::connect(fpFound, &QState::entered, this, &FP_GrowRX305_StateMachine::fpFound);

    m_sm->start();

}

QString FP_GrowRX305_StateMachine::command() const
{
    return m_command;
}

void FP_GrowRX305_StateMachine::setCommand(const QString &command)
{
    m_command = command;
}

int FP_GrowRX305_StateMachine::getIdToEnroll() const
{
    return m_idToEnroll;
}

void FP_GrowRX305_StateMachine::setIdToEnroll(int idToEnroll)
{
    m_idToEnroll = idToEnroll;
}

void FP_GrowRX305_StateMachine::fpFound()
{
    emit fingerprintFound();
}

void FP_GrowRX305_StateMachine::getImgCmd()
{
    setCommand("EF01ffffffff010003010005");
    emit sendCommand();
}

void FP_GrowRX305_StateMachine::gencharCommand1()
{
    setCommand("EF01ffffffff01000402010008");
    emit sendCommand();
}

void FP_GrowRX305_StateMachine::gencharCommand2()
{
    setCommand("EF01ffffffff01000402020009");
    emit sendCommand();
}

void FP_GrowRX305_StateMachine::regModelCmd()
{
    setCommand("EF01ffffffff010003050009");
    emit sendCommand();
}

void FP_GrowRX305_StateMachine::storeCmd()
{
    QString idHexa = QString("%1").arg(m_idToEnroll, 4, 16, QChar('0')).toUpper();
    QString storeCmd = "EF01ffffffff0100060601";

    int idHighByte = (m_idToEnroll >> 8) & 0xFFU;
    int idLowerByte = m_idToEnroll & 0xFFU;

    int checksum = 14 + idHighByte + idLowerByte;
    qDebug()<< " CHECSUM " << checksum;
    QString checksumHexa = QString("%1").arg(checksum, 4, 16, QChar('0')).toUpper();

    setCommand(storeCmd+idHexa+checksumHexa);
    emit sendCommand();
}

void FP_GrowRX305_StateMachine::searchCmd()
{
    setCommand("EF01ffffffff0100080401000003E900FA");
    emit sendCommand();
}
