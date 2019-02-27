#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <list>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include "actuatordefine.h"
#include "communicateunit.h"


//class CommunicateUnit;

class Communication
{

public:
    static Communication *getInstance(int nType);
    explicit Communication();
    //void sendDataBeforeDestroyed();
    virtual ~ Communication();
    virtual int addCommunication(const std::string &str,const uint32_t num)=0;
    void sendData(uint8_t nDeviceId,const std::vector<uint8_t> & data);
    void sendDataWithLongId(uint64_t longId,const std::vector<uint8_t> & data);
    void sendData(const std::string& unitName,const std::vector<uint8_t> & data);
    bool hasDataWaitToSend();
    void stop();
    void recordRemainCmds();
    void addRelateIdToUnit(uint32_t nUnitId,uint8_t nRelateId);
    virtual void removeUnAvailablePorts()=0;
    void changeUnitRelateId(uint8_t nOldId,uint8_t nNewId);
    void setUnitConnectionStatus(uint32_t nUnitId,uint8_t nStatus);
//public slots:
    void unitFinished(uint8_t unitId);
    std::string getCommunicateUnitName(uint32_t unitId)const;
    void setSoftwareVersion(uint16_t version,uint32_t nUnitId);
    uint16_t readSoftwareVersion(uint32_t nUnitId)const;
    void setHardwareVersion(uint16_t version,uint32_t nUnitId);
    uint16_t readHardwareVersion(uint32_t nUnitId)const;
protected:

//signals:
public:
    void response(uint32_t unitId, std::vector<uint8_t>& response);
    //void request(const std::vector<uint8_t> &request);
    void connectionError(uint8_t unitId,uint16_t errorId,std::string errorStr);
private:
    std::mutex m_responstMutex;
protected:
    std::vector<CommunicateUnit *> m_lUnits;
};

#endif // COMMUNICATION_H
