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

    int getFingerprintId() const;

private:
    QStateMachine *m_sm;
    QString m_command;
    int m_fingerprintId;

signals:
    void enroll(int id);
    void success();
    void error();
    void sendCommand();
    void search();
    void cancel();
    void fingerprintFound();
    void eraseFingerprint(int id);


public slots:
    void getImgCmd();
    void gencharCommand1();
    void gencharCommand2();
    void regModelCmd();
    void storeCmd();
    void searchCmd();
    void setFingerprintId(int id);
    void fpFound();
    void eraseCmd();

};

#endif // FP_GROWRX305_STATEMACHINE_H
