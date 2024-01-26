#ifndef FP_SERIALCOMUNICATION_H
#define FP_SERIALCOMUNICATION_H

#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>

class FP_SerialComunication : public QThread
{
    Q_OBJECT
public:
    explicit FP_SerialComunication(QObject *parent = nullptr);

    void loop(bool *cancelFlag);
    void commandToSend(QString command);
    void sendCommand();
    QByteArray getResponse();

    QString getCommandToSend() const;

signals:
    void response(QString response);
    void timeout();

private:
    QString m_commandToSend;
    QByteArray m_response;
    QString m_portName;

    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;

    // QThread interface
protected:
    void run() override;
};

#endif // FP_SERIALCOMUNICATION_H
