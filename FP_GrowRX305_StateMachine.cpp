#include "FP_GrowRX305_StateMachine.h"
#include "FP_GrowRX305.h"

FP_GrowRX305_StateMachine::FP_GrowRX305_StateMachine(QObject *parent) : QObject(parent)
{
    m_sm = new QStateMachine(this);

    QState *idle = new QState(m_sm);

    QState *enroll = new QState(m_sm);
    QState *gettingImage = new QState(enroll);
    QState *generatingChar = new QState(enroll);
    QState *registringModel = new QState(enroll);
    QState *storing = new QState(enroll);

    m_sm->setInitialState(idle);

    idle->addTransition(this, &FP_GrowRX305_StateMachine::signalOne, enroll);
    enroll->setInitialState(gettingImage);


    gettingImage->addTransition(this, &FP_GrowRX305_StateMachine::success, generatingChar);
    gettingImage->addTransition(this, &FP_GrowRX305_StateMachine::error, gettingImage);

    QObject::connect(gettingImage, &QState::entered, this, &FP_GrowRX305_StateMachine::getImgCmd);
    QObject::connect(generatingChar, &QState::entered, this, &FP_GrowRX305_StateMachine::gencharCommand);

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

void FP_GrowRX305_StateMachine::getImgCmd()
{
    setCommand("EF01ffffffff010003010005");
    emit sendCommand();
}

void FP_GrowRX305_StateMachine::gencharCommand()
{
    setCommand("EF01ffffffff01000402010008");
    emit sendCommand();
}
