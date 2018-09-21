#ifndef SERIALCOMMUNICATEUNIT_H
#define SERIALCOMMUNICATEUNIT_H
#include "communicateunit.h"
#include "asio/serial_port.hpp"

class SerialCommunicateUnit : public CommunicateUnit
{
    
public:
    SerialCommunicateUnit(uint32_t unitId,const std::string portName,uint32_t bauRate);
    ~SerialCommunicateUnit();
    std::string getCommunicationUnitName()const override{return m_sPortName;}
//public slots:
    virtual void progress();
    void receiveHandler(const asio::error_code & ec,std::size_t bytes);
private:
    std::string m_sPortName;
    uint32_t m_nBauRate;
    asio::serial_port *m_pSerialPort;
    char m_receiveBuf[1024];//only receive from serialport
    std::vector<uint8_t> m_receiveArray;//send to user
};

#endif // SERIALCOMMUNICATEUNIT_H
