#ifndef VERSIONNUMBER_H
#define VERSIONNUMBER_H
#include "v8stdint.h"
#include <string>

class VersionNumber
{
public:
    VersionNumber(uint32_t major,uint32_t second, uint32_t release);
    std::string toStdString()const;
private:
    uint32_t m_major;
    uint32_t m_second;
    uint32_t m_release;
};

#endif // VERSIONNUMBER_H
