#ifndef INNFOSPROXY_H
#define INNFOSPROXY_H


#include <vector>
#include "idata.h"
#include <vector>
#include <mutex>
#include "actuatordefine.h"
#include "mediator.h"

class QFile;
class QTextStream;


//串口协议(有数据有校验)
class InnfosProxy
{

public:
    IData * m_pData;
//public slots:
    void Send();
    void Send(const std::string & target);
    void Send(uint64_t longId);
public:
    InnfosProxy(const int nId,const uint8_t nDeviceId);
    virtual ~InnfosProxy();
    static void decode(uint32_t communicateUnitId,std::vector<uint8_t> &buf);
    const int GetIdx()const { return m_nId; }
    static void SendProxy(const uint8_t nDeviceId,const int nIdx);
    static void SendProxy(const uint8_t nDeviceId,const uint32_t nDeviceMac,const int nIdx,const uint8_t newDeviceId);
    static void SendProxyWithLongId(const uint64_t longId,const int nIdx);
    static void SendProxyWithLongId(const uint64_t longId,const uint32_t nDeviceMac,const int nIdx,const uint8_t newDeviceId);
    template <class T>
    static void SendProxy(const uint8_t nDeviceId,const int nIdx,T data){
        InnfosProxy proxy(nIdx,nDeviceId);
        proxy.m_pData->WriteShort(sizeof(T));
        proxy.m_pData->WriteData(data);
        proxy.m_pData->AddCRCCode(sizeof(T));
        proxy.m_pData->AddProxyEnd();
        proxy.Send();
    }
    template <class T>
    static void SendProxyWithLongId(const uint64_t longId,const int nIdx,T data){
        InnfosProxy proxy(nIdx,Mediator::toDeviceId(longId));
        proxy.m_pData->WriteShort(sizeof(T));
        proxy.m_pData->WriteData(data);
        proxy.m_pData->AddCRCCode(sizeof(T));
        proxy.m_pData->AddProxyEnd();
        proxy.Send(longId);
    }
    template <class T>
    static void SendProxy(const std::string & target,const int nIdx,T data){
        InnfosProxy proxy(nIdx,0);
        proxy.m_pData->WriteShort(sizeof(T));
        proxy.m_pData->WriteData(data);
        proxy.m_pData->AddCRCCode(sizeof(T));
        proxy.m_pData->AddProxyEnd();
        proxy.Send(target);
    }

    static std::vector<uint8_t> getProxyContent(const uint8_t nDeviceId,const int nProxyIdx);
    enum TMP_DIRECTIVES{
        T_D_READ_QUATERNION=0x01,//读取四元数
        T_D_READ_ALL_QUATERNION=0x02,
        T_D_READ_VERSION=0x04,
        T_D_READ_NRF=0x05,
    };
protected:
private:


    static void decodeTmpCmd(uint32_t communicateUnitId,std::vector<uint8_t> &buf);//解析与中间板的协议指令，指令内容里还有指令符
private:
    int m_nId;//协议id
protected:
};

//无数据协议，主要是为了获取一些数据
class NoDataProxy : public InnfosProxy
{

//public slots:
public:
    NoDataProxy(const int nId, const uint8_t nDeviceId);
    static void decode(uint32_t communicateUnitId,std::vector<uint8_t> &buf);
protected:
private:
};

//有数据无校验
class NoCrcProxy : public InnfosProxy
{

//public slots:
public:
    NoCrcProxy(const int nId, const uint8_t nDeviceId);
    static void decode(uint32_t communicateUnitId,std::vector<uint8_t> &buf);
protected:
private:
};

//proxy watcher,use to log those proxys without response
//class ProxyWatcher
//{

//public:
//    static ProxyWatcher * getInstance();
//    static void autoDestroy();
//    void addSendItem(const std::vector<uint8_t> & buf);
//    void reciveItem(const std::vector<uint8_t> & buf);
//    virtual ~ProxyWatcher();
//protected:
//    void checkTimeout();
//    ProxyWatcher(QObject *parent=Q_NULLPTR);
//    struct logStruct{
//        QTime sendTime;
//        std::vector<uint8_t> sendContent;
//    };
//private:
//    static ProxyWatcher * m_pWatcher;
//    std::vector<logStruct *> m_sendContents;
//    QFile * m_pLogFile;
//    QTextStream * m_pLogStream;
//    std::mutex m_qmMutex;
//};
#endif // INNFOSPROXY_H
