#ifndef FP_WORKER_H
#define FP_WORKER_H

#include <QObject>

#include "FP_SerialComunication.h"

class FP_Worker : public QObject
{
    Q_OBJECT
public:
    FP_Worker() = default;

signals:
    void done();

public slots:
    void cancel() { m_cancel = true; }
    void doWork(FP_SerialComunication *fp);


private:
    bool m_cancel = false;

};

#endif // FP_WORKER_H
