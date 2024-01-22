#include "FP_Worker.h"

void FP_Worker::doWork(FP_SerialComunication *fp)
{
    fp->loop(&m_cancel);
    qDebug() << "Worker done..";
    emit done();
}
