#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H
#include "communication.h"

class SerialCommunication : public Communication
{
public:
    SerialCommunication();
    ~SerialCommunication();
    virtual int addCommunication(const std::string &str,const uint32_t num);
    virtual void removeUnAvailablePorts();
private:
    std::vector<int> m_vConnection;
};


#endif // SERIALCOMMUNICATION_H
