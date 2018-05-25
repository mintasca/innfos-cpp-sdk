#ifndef SERIALCOMMUNICATEUNIT_H
#define SERIALCOMMUNICATEUNIT_H
#include "communicateunit.h"

class SerialCommunicateUnit : public CommunicateUnit
{
    
public:
    SerialCommunicateUnit(uint32_t unitId,const std::string portName,uint32_t bauRate);
    std::string getCommunicationUnitName()override{return m_sPortName;}
//public slots:
    virtual void progress();
private:
    std::string m_sPortName;
    uint32_t m_nBauRate;
};

#endif // SERIALCOMMUNICATEUNIT_H
