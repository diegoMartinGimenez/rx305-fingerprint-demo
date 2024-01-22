#ifndef FINGERPRINTCOMMUNICATIONPROTOCOL_H
#define FINGERPRINTCOMMUNICATIONPROTOCOL_H


class FingerPrintCommunicationProtocol
{        
public:
    virtual bool open() = 0;
    virtual void cancel() = 0;
    virtual bool enroll(int id) = 0;



};

#endif // FINGERPRINTCOMMUNICATIONPROTOCOL_H
