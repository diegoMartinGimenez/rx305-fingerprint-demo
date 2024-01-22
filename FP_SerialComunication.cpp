#include "FP_SerialComunication.h"
#include <QThread>
#include <QAbstractEventDispatcher>

FP_SerialComunication::FP_SerialComunication(QObject *parent) : QObject(parent)
{
    m_serialport.setBaudRate(QSerialPort::Baud57600);
    m_serialport.setDataBits(QSerialPort::Data8);
    m_serialport.setParity(QSerialPort::NoParity);
    m_serialport.setStopBits(QSerialPort::OneStop);
    m_serialport.setFlowControl(QSerialPort::NoFlowControl);
    m_portName = "/dev/ttyUSB0";

}

bool FP_SerialComunication::open()
{
    m_serialport.setPortName(m_portName);
    if (m_serialport.open(QIODevice::ReadWrite)){
        qDebug() << "Serial port opened successfully.";
        return true;

    } else {
        qDebug() << "Failed to open serial port:" << m_serialport.errorString();
        return false;
    }
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

    QSerialPort m_serial;
    m_serial.setPortName(m_portName);
    m_serial.setBaudRate(QSerialPort::Baud57600);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if (m_serial.open(QIODevice::ReadWrite))
        qDebug() << "open port success";
    else {
        qDebug() << "error al abir com";
        return;
    }

    QByteArray requestData;
    requestData.append(QByteArray::fromHex(m_commandToSend.toUtf8()));

    m_serial.write(requestData);
    if (m_serial.waitForBytesWritten(1000)) {
        // read response
        if (m_serial.waitForReadyRead(1000)) {
            m_response = m_serial.readAll();
            while (m_serial.waitForReadyRead(10)){
                m_response += m_serial.readAll();
            }
            qDebug() << "Response: " << m_response.toHex() << this << Q_FUNC_INFO << QThread::currentThread();
            emit this->response(m_response.toHex());
         } else {
//            emit timeout(tr("Wait read response timeout %1").arg(QTime::currentTime().toString()));
            qDebug() << "Timeout";
        }
    } else {
//        emit timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
        qDebug() << "Timeout";

    }


}

QByteArray FP_SerialComunication::getResponse()
{
    return m_response;
}

