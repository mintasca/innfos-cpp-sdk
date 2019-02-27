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
#include <sstream>
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
    void SendRequest(uint64_t longId,const std::vector<uint8_t> & buf);
    void SendRequest(const std::string& target,const std::vector<uint8_t> & buf);
    void Handshake(uint64_t longId,bool bSuccess);
    void SetCurParam(const uint64_t longId,const double value, const int nProxyId);//set motor param values
    void SetSucceed(const uint64_t longId, const int nProxyId);//
    void SetFailed(const uint64_t longId, const int nProxyId);//设置下位机参数fail
    //void NullChartPointer();

    void reciveMotorInfo(uint32_t communicateUnitId,const uint32_t nDeviceMac, const uint8_t nDeviceId);
    void receiveNoDataProxy(const int nDeviceID);
    void checkServosStatus();//check servos are on or off
    void recognizeFinished(std::multimap<std::pair<uint8_t,uint32_t>,uint32_t> motorsInfo);
    void chartVauleChange(const int nChannelId,double values);//only use by chart
    asio::io_context * ioContext();
#ifdef IMU_ENABLE
    void receiveQuaternion(uint64_t imuId, double w,double x,double y,double z);
    void requestQuaternion(uint8_t nCmdId,uint64_t imuId=0);
    void requestLossRatio();
    void receiveLossRatio(uint64_t imuId,uint32_t receive,uint32_t lost);
#endif
    std::string versionString()const;
    int addCommunicationUnit(std::string unitStr,uint32_t unitNumber);
    void setUnitConnectionStatus(uint32_t nUnitId,uint8_t nStatus);
    void initCommunication(int nType);
//public slots:
    void response(uint32_t nUnitId,const std::vector<uint8_t> buf);
    void reconnectDevice(uint64_t longId);
    void errorOccur(uint64_t longId,uint16_t errorId, std::string errorStr);
    void motorAttrChanged(uint64_t longId,uint8_t nAttrId,double value);
    void receivePanelVersion(uint32_t communicationId,uint16_t softVersion,uint16_t hardVersion);
    static uint64_t toLongId(uint32_t communicationId, uint8_t byteId);
    static uint32_t toCommunicationId(uint64_t longId);
    static uint8_t toDeviceId(uint64_t longId);
    static std::string toString(uint64_t longId);
protected:
    Mediator();
public:
    void handleIO();
    void runOnce();
private:
    void writeLog(bool bForce = false);
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
    CSignal<uint64_t,uint8_t,double> m_sRequestBack;
    CSignal<uint64_t,uint16_t,std::string> m_sError;
    CSignal<uint64_t,uint8_t,double> m_sMotorAttrChanged;
    CSignal<> m_sNewChartStart;
    CSignal<uint8_t,double> m_sChartValueChange;
#ifdef IMU_ENABLE
    CSignal<uint64_t,double,double,double,double> m_sQuaternion;
    CSignal<uint64_t,uint32_t,uint32_t> m_sLossRatio;
#endif
private:
    VersionNumber * m_pVersionMgr;
    AbstractAutoRecognize * m_pRecognize;
    Communication * m_pCommunication;
    asio::io_context * m_pIoContext;
    std::thread * m_pIoThread;
    bool m_bStop;
    std::mutex m_ioMutex;
    std::stringstream logStr;
};

#endif // MEDIATOR_H
