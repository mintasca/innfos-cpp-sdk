#ifndef PROXYPARSER_H
#define PROXYPARSER_H
#include <sstream>
#include "idata.h"

class ProxyParser 
{
    
public:
    static ProxyParser * getInstance();
    ~ProxyParser();
    void parse(uint32_t communicateUnitId,const std::vector<uint8_t> & buf);
protected:
    explicit ProxyParser();
    void handleError();//data error
    bool headCheck(const std::vector<uint8_t> & data);
    bool dataCheck(const std::vector<uint8_t> & data);
    void dispatchData(uint32_t communicateUnitId,std::vector<uint8_t> & buf);
    void writeLog(bool bForce = false);
//signals:
private:
    class GC{
    public:
        ~GC()
        {
            if(m_pParser!=nullptr)
            {
                delete m_pParser;
                m_pParser = nullptr;
            }
        }
        static GC gc;
    };

//public slots:
private:
    std::vector<uint8_t> m_remainData;//data wait for parse;
    static ProxyParser * m_pParser;
    std::stringstream logStr;
};

#endif // PROXYPARSER_H
