#include "FP_Worker.h"

void FP_Worker::doWork(FP_SerialComunication *fp)
{
    qDebug() << "command: " << fp->getCommandToSend();
//    fp->loop(&m_cancel);
//    qDebug() << "Worker done..";
//    emit done();
    fp->sendCommand();
    emit done();

}
