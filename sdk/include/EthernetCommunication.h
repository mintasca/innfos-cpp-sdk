#ifndef EHTERCOMMUNICATION_H
#define EHTERCOMMUNICATION_H
#include "communication.h"
#include <vector>

class EthernetCommunication : public Communication
{
public:
    EthernetCommunication();
    ~EthernetCommunication();
    virtual int addCommunication(const std::string &str,const uint32_t num);
    virtual void removeUnAvailablePorts();
private:
    std::vector<int> m_vConnection;
};

#endif // EHTERCOMMUNICATION_H
