#ifndef COMMUNICATEUNIT_H
#define COMMUNICATEUNIT_H

#include <map>
#include <vector>
#include <list>
#include <mutex>
#include "CSignal.hpp"

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
    void stopCommunication();
    bool hasDataWaiting();
    bool isAvailable()const;
    void setConnectionStatus(uint8_t nStatus);
    uint8_t getConnectionStatus()const;
    virtual std::string getCommunicationUnitName()=0;
//public slots:
    virtual void progress()=0;
protected:

    std::map<uint8_t,std::vector <std::vector<uint8_t>>> m_dataMap;
    std::vector<std::vector<uint8_t>> m_dataVector;
    std::mutex m_qmMutex;
    bool m_bStop;
    uint32_t m_nUnitId;
    uint8_t m_nConnectionStatus;
    std::list<uint8_t> m_relateIdList;//motors's ids whitch communicate via this unit
    //asio m_unitAddr;
public:
    CSignal<uint32_t,std::vector<uint8_t>& > m_sResponse;

};

#endif // COMMUNICATEUNIT_H
