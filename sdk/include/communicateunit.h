#ifndef COMMUNICATEUNIT_H
#define COMMUNICATEUNIT_H

#include <map>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>
#include "CSignal.hpp"
#include <thread>

class CommunicateUnit
{
public:
    explicit CommunicateUnit(uint32_t unitId);
    virtual ~CommunicateUnit();
//signals:
    void response(uint32_t unitId,std::vector<uint8_t> responseData);
    void error(std::string errorString);
    void finished(uint32_t unitId);
public:
    void addRelateId(uint8_t id);
    void switchOldRelateIdToNewRelateId(uint8_t oldId,uint8_t newId);
    void setRelateIdList(const std::list<uint8_t> &idList);
    bool isContainsRelateId(const uint8_t id)const;
    uint32_t getUnitId()const{
        return m_nUnitId;
    }
    void sendData(const std::vector<uint8_t> & sendData);

    bool hasDataWaiting();
    bool isAvailable()const;
    void setConnectionStatus(uint8_t nStatus);
    uint8_t getConnectionStatus()const;
    virtual std::string getCommunicationUnitName()const=0;
//public slots:
    virtual void progress()=0;
    void setSoftwareVersion(uint16_t version);
    uint16_t readSoftwareVersion()const;
    void setHardwareVersion(uint16_t version);
    uint16_t readHardwareVersion()const;
    void stopCommunication();
protected:

    std::map<uint8_t,std::vector <std::vector<uint8_t>>> m_dataMap;
    std::vector<std::vector<uint8_t>> m_dataVector;
    std::vector<std::vector<uint8_t>> m_dataVectorFast;//datas in this vector will be send very fast
    std::mutex m_qmMutex;
    std::mutex m_dataMutex;
    std::condition_variable wait_cond;
    bool m_bStop;
    std::thread *m_pCommunicateThread;
    uint32_t m_nUnitId;
    uint8_t m_nConnectionStatus;
    std::list<uint8_t> m_relateIdList;//motors's ids whitch communicate via this unit
    uint16_t m_hardwareVersion;
    uint16_t m_softwareVersion;
    //asio m_unitAddr;
public:
    CSignal<uint32_t,std::vector<uint8_t>& > m_sResponse;

};

#endif // COMMUNICATEUNIT_H
