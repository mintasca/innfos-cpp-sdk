#ifndef AUTORECOGINZE_H
#define AUTORECOGINZE_H
#include "asio/asio/ip/udp.hpp"
#include "AbstractAutoRecognize.h"
#include <map>
#include <vector>


using asio::ip::udp;

class EthernetAutoRecognize : public AbstractAutoRecognize
{
public:
    explicit EthernetAutoRecognize();
    ~EthernetAutoRecognize();
    void startRecognize()override;
    void waitTimeout()override;
protected:

    void findCommunicationUnits();
//signals:

//public slots:

//private slots:
private:
    void onIpBroadcast();
    void receiveHandler(const asio::error_code & ec,std::size_t size);
private:

private:
    bool m_bFindAvaliable;
    udp::socket * m_pSocket;
    std::vector<asio::ip::address> m_addrVector;
    uint8_t m_buffer[1024];
    udp::endpoint m_remoteEndPoint;
};

#endif // AUTORECOGINZE_H
