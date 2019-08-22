#ifndef DATAUTIL_H
#define DATAUTIL_H
#include <map>
//#include "innfosproxy.h"
#include "actuatordata.h"
#include "actuatordefine.h"


class DataUtil
{
public:
    static Actuator::Directives convertToReadProxyId(Actuator::ActuatorAttribute id);
    static Actuator::Directives convertToSetProxyId(Actuator::ActuatorAttribute id);
    static Actuator::ActuatorAttribute convertToMotorDataId(Actuator::Directives id);
    ~DataUtil();
private:
private:
    class GC{
    public:
        ~GC()
        {
            if(m_pInstance!=nullptr)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
            }
        }
        static GC gc;
    };
private:
    DataUtil();
    std::map<Actuator::Directives,Actuator::ActuatorAttribute> readDataMap;
    std::map<Actuator::Directives,Actuator::ActuatorAttribute> setDataMap;
    static DataUtil * m_pInstance;
};
#endif // DATAUTIL_H
