#include "FP_SerialComunication.h"
#include <QThread>
#include <QAbstractEventDispatcher>

FP_SerialComunication::FP_SerialComunication(QObject *parent) : QThread(parent), waitTimeout(1000), quit(false)
{
    m_portName = "/dev/ttyUSB0";
}

void FP_SerialComunication::loop(bool *cancelFlag)
{
    int counter = 1;
    while(true){
        qDebug() << this << "count: " << counter++ << Q_FUNC_INFO << QThread::currentThread();
        QThread::currentThread()->eventDispatcher()->processEvents(QEventLoop::ProcessEventsFlag::AllEvents);
        if (*cancelFlag) {
            qDebug() << "Canceled ";
            return;
        }
        sendCommand();

        QThread::msleep(500);
    }

}

void FP_SerialComunication::commandToSend(QString command)
{
    m_commandToSend = command;
}

void FP_SerialComunication::sendCommand()
{
    qDebug() << "Command: " << m_commandToSend;
    QMutexLocker locker(&mutex);
    qDebug() << this << Q_FUNC_INFO << QThread::currentThread();
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}

QByteArray FP_SerialComunication::getResponse()
{
    return m_response;
}

QString FP_SerialComunication::getCommandToSend() const
{
    return m_commandToSend;
}

void FP_SerialComunication::run()
{
    mutex.lock();
    int currentWaitTimeout = waitTimeout;
    QString currentRequest = m_commandToSend;
    mutex.unlock();

    QSerialPort serial;

    serial.setPortName(m_portName);
    serial.setBaudRate(QSerialPort::Baud57600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::TwoStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadWrite)) {
//        emit error(tr("Can't open %1, error code %2").arg(m_portName).arg(serial.error()));
        qDebug() << tr("Can't open %1, error code %2").arg(m_portName).arg(serial.error());
        return;
    }
    QThread::currentThread()->setObjectName("Secondary thread");

    while (!quit) {
        // write request
        QByteArray requestData;
        requestData.append(QByteArray::fromHex(currentRequest.toUtf8()));
        serial.write(requestData);        
        if (serial.waitForBytesWritten(currentWaitTimeout)) {
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout)) {
                m_response = serial.readAll();
                 while (serial.waitForReadyRead(10)){
                    m_response += serial.readAll();
                }
                QString response(m_response);
                qDebug() << "Response: " << m_response.toHex() << this << Q_FUNC_INFO << QThread::currentThread();
                emit this->response(m_response.toHex());
            } else {
                qDebug() << "Timeout read response";
                emit timeout();
            }
        } else {
            qDebug() << "Timeout write request";
            emit timeout();
        }
        mutex.lock();
        cond.wait(&mutex);
        currentRequest = m_commandToSend;
        mutex.unlock();
//        QThread::msleep(100);

    }

}

