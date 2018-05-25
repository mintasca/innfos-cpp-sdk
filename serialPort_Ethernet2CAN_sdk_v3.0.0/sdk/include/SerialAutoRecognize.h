#ifndef SERIALAUTORECOGNIZE_H
#define SERIALAUTORECOGNIZE_H
#include <vector>
#include "AbstractAutoRecognize.h"
#include "serial/serial.h"

class SerialAutoRecognize : public AbstractAutoRecognize
{

public:
    explicit SerialAutoRecognize();
    void startRecognize()override;
    void waitTimeout()override;
private:
    void findAvailablePorts();
private:
    std::vector <serial::PortInfo> m_portList;
};

#endif // SERIALAUTORECOGNIZE_H
