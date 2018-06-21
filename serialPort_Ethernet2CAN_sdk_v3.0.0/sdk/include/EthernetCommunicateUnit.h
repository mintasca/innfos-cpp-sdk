﻿#ifndef ETHERNETCOMMUNICATEUNIT_H
#define ETHERNETCOMMUNICATEUNIT_H
#include "communicateunit.h"
#include "asio/ip/udp.hpp"
#include <vector>

class EthernetCommunicateUnit : public CommunicateUnit
{
    
public:
    EthernetCommunicateUnit(uint32_t unitId,const std::string unitAddr,uint32_t port);
    ~EthernetCommunicateUnit();
    std::string getCommunicationUnitName()override{return m_sUnitAddr;}
    void progress();
    void receiveHandler(const asio::error_code & ec,std::size_t bytes);
private:
    std::string m_sUnitAddr;
    uint16_t m_nPort;
    asio::ip::udp::socket * m_pSocket;
    char m_receiveBuf[1024];//only receive from socket
    std::vector<uint8_t> m_receiveArray;//send to user
};

#endif // ETHERNETCOMMUNICATEUNIT_H
