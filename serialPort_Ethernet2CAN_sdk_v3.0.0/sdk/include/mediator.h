#ifndef MEDIATOR_H
#define MEDIATOR_H
#include "asio/io_context.hpp"
#include "actuatordata.h"
#include <functional>
#include <string>
#include "CSignal.hpp"
#include "AbstractAutoRecognize.h"
#include "communication.h"
#include "actuatordefine.h"
#include <thread>
#include <mutex>
#include "versionnumber.h"
//#define TEST_DEBUG
#define LOG_DEBUG
//#define NO_HEART_BEAT
//#define WARNING_DEBUG


const double velScale = 6000;
const double curScale = 8.25;


#define mediator  Mediator::getInstance()//get instance of Mediator
#define requestCallback  std::function<void (uint8_t,uint8_t,double)>
#define errorInfoFunc std::function<void (uint8_t,uint16_t,std::string)>

class Mediator
{
public:
    static Mediator * getInstance();
    ~Mediator();
    void autoRecognize();//auto recognize motor
    void onCanConnected(uint32_t nCommunicationUnitId);
    void SendRequest(const std::vector<uint8_t> & buf);
    void Handshake(uint32_t nDeviceId,bool bSuccess);
    void SetCurParam(const int nDeviceID,const double value, const int nProxyId);//set motor param values
    void SetSucceed(const uint8_t nDeviceId, const int nProxyId);//
    void SetFailed(const uint8_t nDeviceId, const int nProxyId);//设置下位机参数fail
    //void NullChartPointer();

    void reciveMotorInfo(uint32_t communicateUnitId,const uint32_t nDeviceMac, const uint8_t nDeviceId);
    void receiveNoDataProxy(const int nDeviceID);
    void checkServosStatus();//check servos are on or off
    void recognizeFinished(std::map<uint8_t,uint32_t> motorsInfo);
    void chartVauleChange(const int nChannelId,double values);//only use by chart
    asio::io_context * ioContext();
#ifdef IMU_ENABLE
    void receiveQuaternion(uint8_t imuId, double w,double x,double y,double z);
    void requestQuaternion(uint8_t nCmdId);
#endif
    std::string versionString()const;
    int addCommunicationUnit(std::string unitStr,uint32_t unitNumber);
    void setUnitConnectionStatus(uint32_t nUnitId,uint8_t nStatus);
    void initCommunication(int nType);
//public slots:
    void response(uint32_t nUnitId,const std::vector<uint8_t> buf);
    void reconnectDevice(uint8_t nDeviceId);
    void errorOccur(uint8_t nDeviceId,uint16_t errorId, std::string errorStr);
    void motorAttrChanged(uint8_t nDeviceId,uint8_t nAttrId,double value);
protected:
    Mediator();
public:
    void handleIO();
private:
    static Mediator *m_pInstance;
private:
    class GC{
    public:
        ~GC()
        {
            if(m_pInstance!=nullptr)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
            }
        }
        static GC gc;
    };
public:
    CSignal<> m_sRecognizeFinished;
    CSignal<uint8_t,uint8_t,double> m_sRequestBack;
    CSignal<uint8_t,uint16_t,std::string> m_sError;
    CSignal<uint8_t,uint8_t,double> m_sMotorAttrChanged;
    CSignal<> m_sNewChartStart;
    CSignal<uint8_t,double> m_sChartValueChange;
#ifdef IMU_ENABLE
    CSignal<uint8_t,double,double,double,double> m_sQuaternion;
#endif
private:
    VersionNumber * m_pVersionMgr;
    AbstractAutoRecognize * m_pRecognize;
    Communication * m_pCommunication;
    asio::io_context * m_pIoContext;
    std::thread * m_pIoThread;
    bool m_bStop;
    std::mutex m_ioMutex;
};

#endif // MEDIATOR_H
