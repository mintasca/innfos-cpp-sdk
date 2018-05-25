#ifndef ETHERNETCOMMUNICATEUNIT_H
#define ETHERNETCOMMUNICATEUNIT_H
#include "communicateunit.h"

class EthernetCommunicateUnit : public CommunicateUnit
{
    
public:
    EthernetCommunicateUnit(uint32_t unitId,const std::string unitAddr,uint32_t port);
    std::string getCommunicationUnitName()override{return m_sUnitAddr;}
    void progress();
private:
    std::string m_sUnitAddr;
    uint16_t m_nPort;
};

#endif // ETHERNETCOMMUNICATEUNIT_H
