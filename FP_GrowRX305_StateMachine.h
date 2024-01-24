#ifndef FP_GROWRX305_STATEMACHINE_H
#define FP_GROWRX305_STATEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QState>

class FP_GrowRX305_StateMachine : public QObject
{
    Q_OBJECT
public:
    explicit FP_GrowRX305_StateMachine(QObject *parent = nullptr);
    QString command() const;
    void setCommand(const QString &command);

private:
    QStateMachine *m_sm;
    QString m_command;

signals:
    void signalOne();
    void success();
    void error();
    void sendCommand();

public slots:
    void getImgCmd();
    void gencharCommand();
};

#endif // FP_GROWRX305_STATEMACHINE_H
