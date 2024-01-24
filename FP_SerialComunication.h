#ifndef FP_SERIALCOMUNICATION_H
#define FP_SERIALCOMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

class FP_SerialComunication : public QObject
{
    Q_OBJECT
public:
    explicit FP_SerialComunication(QObject *parent = nullptr);
    bool open();
    void loop(bool *cancelFlag);
    void commandToSend(QString command);
    void sendCommand();
    QByteArray getResponse();

    QString getCommandToSend() const;

signals:
    void response(QString response);

private:
    QSerialPort m_serialport;
    QString m_commandToSend;
    QByteArray m_response;
    QString m_portName;

};

#endif // FP_SERIALCOMUNICATION_H
