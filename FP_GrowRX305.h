#ifndef FP_GROWRX305_H
#define FP_GROWRX305_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include <QAbstractEventDispatcher>
#include <QMap>

#include "FingerPrintCommunicationProtocol.h"
#include "FP_Worker.h"
#include "FP_SerialComunication.h"
#include "FP_GrowRX305_StateMachine.h"

//-------------------------------------------------------------------------//
//Received packet verification status codes from host device

#define FPS_RX_OK                        0x00U  //when the response is correct
#define FPS_RX_BADPACKET                 0x01U  //if the packet received from FPS is badly formatted
#define FPS_RX_WRONG_RESPONSE            0x02U  //unexpected response
#define FPS_RX_TIMEOUT                   0x03U  //when no response was received

//-------------------------------------------------------------------------//
//Command codes

#define FP_CMD_SCANFINGER            0x01U    //scans the finger and collect finger image
#define FP_CMD_IMAGETOCHARACTER      0x02U    //generate char file from a single image and store it to one of the buffers
#define FP_CMD_MATCHTEMPLATES        0x03U    //match two fingerprints precisely
#define FP_CMD_SEARCHLIBRARY         0x04U    //search the fingerprint library
#define FP_CMD_GENERATETEMPLATE      0x05U    //combine both character buffers and generate a template
#define FP_CMD_STORETEMPLATE         0x06U    //store the template on one of the buffers to flash memory
#define FP_CMD_LOADTEMPLATE          0x07U    //load a template from flash memory to one of the buffers
#define FP_CMD_EXPORTTEMPLATE        0x08U    //export a template file from buffer to computer
#define FP_CMD_IMPORTTEMPLATE        0x09U    //import a template file from computer to sensor buffer
#define FP_CMD_EXPORTIMAGE           0x0AU    //export fingerprint image from buffer to computer
#define FP_CMD_IMPORTIMAGE           0x0BU    //import an image from computer to sensor buffer
#define FP_CMD_DELETETEMPLATE        0x0CU    //delete a template from flash memory
#define FP_CMD_CLEARLIBRARY          0x0DU    //clear fingerprint library
#define FP_CMD_SETSYSPARA            0x0EU    //set system configuration register
#define FP_CMD_READSYSPARA           0x0FU    //read system configuration register
#define FP_CMD_SETPASSWORD           0x12U    //set device password
#define FP_CMD_VERIFYPASSWORD        0x13U    //verify device password
#define FP_CMD_GETRANDOMCODE         0x14U    //get random code from device
#define FP_CMD_SETDEVICEADDRESS      0x15U    //set 4 byte device address
#define FP_CMD_PORTCONTROL           0x17U    //enable or disable comm port
#define FP_CMD_WRITENOTEPAD          0x18U    //write to device notepad
#define FP_CMD_READNOTEPAD           0x19U    //read from device notepad
#define FP_CMD_HISPEEDSEARCH         0x1BU    //highspeed search of fingerprint
#define FP_CMD_TEMPLATECOUNT         0x1DU    //read total template count
#define FP_CMD_SCANANDRANGESEARCH    0x32U    //read total template count
#define FP_CMD_SCANANDFULLSEARCH     0x34U    //read total template count

//-------------------------------------------------------------------------//
//Packet IDs

#define FP_ID_STARTCODE              0xEF01U
#define FP_ID_STARTCODEHIGH          0xEFU
#define FP_ID_STARTCODELOW           0x01U
#define FP_ID_COMMANDPACKET          0x01U
#define FP_ID_DATAPACKET             0x02U
#define FP_ID_ACKPACKET              0x07U
#define FP_ID_ENDDATAPACKET          0x08U

//-------------------------------------------------------------------------//

#define FP_DEFAULT_TIMEOUT           2000  //UART reading timeout in milliseconds
#define FP_DEFAULT_PASSWORD          0xFFFFFFFF
#define FP_DEFAULT_ADDRESS           0xFFFFFFFF

class FP_GrowRX305 : public FingerPrintCommunicationProtocol
{
    Q_OBJECT
public:
    explicit FP_GrowRX305();

    // FingerPrintCommunicationProtocol interface
    void cancel() override;
    bool open() override;
    bool enroll(int id) override;

    void processResponse();

    enum class cCodes : qint8 {
        FP_RESP_OK                  = 0x00U,   //command executed successfully
        FP_RESP_RECIEVEERR          = 0x01U,   //packet receive error
        FP_RESP_NOFINGER            = 0x02U,   //no finger detected
        FP_RESP_ENROLLFAIL          = 0x03U,   //failed to enroll the finger
        FP_RESP_OVERDISORDERFAIL    = 0x04U,   //failed to generate character file due to over-disorderly fingerprint image
        FP_RESP_OVERWETFAIL         = 0x05U,   //failed to generate character file due to over-wet fingerprint image
        FP_RESP_OVERDISORDERFAIL2   = 0x06U,   //failed to generate character file due to over-disorderly fingerprint image
        FP_RESP_FEATUREFAIL         = 0x07U,   //failed to generate character file due to over-wet fingerprint image
        FP_RESP_DONOTMATCH          = 0x08U,   //fingers do not match
        FP_RESP_NOTFOUND            = 0x09U,   //no valid match found
        FP_RESP_ENROLLMISMATCH      = 0x0AU,   //failed to combine character files (two character files (images) are used to create a template)
        FP_RESP_BADLOCATION         = 0x0BU,   //addressing PageID is beyond the finger library
        FP_RESP_INVALIDTEMPLATE     = 0x0CU,   //error when reading template from library or the template is invalid
        FP_RESP_TEMPLATEUPLOADFAIL  = 0x0DU,   //error when uploading template
        FP_RESP_PACKETACCEPTFAIL    = 0x0EU,   //module can not accept more packets
        FP_RESP_IMAGEUPLOADFAIL     = 0x0FU,   //error when uploading image
        FP_RESP_TEMPLATEDELETEFAIL  = 0x10U,   //error when deleting template
        FP_RESP_DBCLEARFAIL         = 0x11U,   //failed to clear fingerprint library
        FP_RESP_WRONGPASSOWRD       = 0x13U,   //wrong password
        FP_RESP_IMAGEGENERATEFAIL   = 0x15U,   //fail to generate the image due to lackness of valid primary image
        FP_RESP_FLASHWRITEERR       = 0x18U,   //error when writing flash
        FP_RESP_NODEFINITIONERR     = 0x19U,   //no definition error
        FP_RESP_INVALIDREG          = 0x1AU,   //invalid register number
        FP_RESP_INCORRECTCONFIG     = 0x1BU,   //incorrect configuration of register
        FP_RESP_WRONGNOTEPADPAGE    = 0x1CU,   //wrong notepad page number
        FP_RESP_COMPORTERR          = 0x1DU,   //failed to operate the communication port
    };
    Q_ENUM(cCodes)

    QString cCodeToString(cCodes code);

public slots:
    void sendCommandToWorker();

private:
    FP_Worker *m_worker;
    FP_SerialComunication *m_port;
    FP_GrowRX305_StateMachine *m_stateMachine;
    QThread *m_thread;



signals:
    void commandSuccess();
    void commandError(QString error);
    void enrolling();


};

#endif // FP_GROWRX305_H
