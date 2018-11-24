#ifndef IDATA_H
#define IDATA_H
#include "v8stdint.h"
#include <vector>

class IData
{
public:
    static uint8_t deviceId;
    void AddProxyEnd();
    void AddProxyBegin(const uint8_t nMode,const uint8_t nDeviceId);
    bool CheckData();
    void AddCRCCode(int16_t nLen);
public:
    IData();
    IData(const std::vector<uint8_t> & buf);
    virtual ~IData() {}
    std::vector<uint8_t> GetBuffer()const {
        return m_buffer;
    }
    int16_t ReadShort();
    uint16_t ReadUShort();
    int8_t ReadByte();
    uint8_t ReadUByte();
    int32_t ReadInt();
    int32_t ReadThreeBytesAsInt();
    int32_t ReadTwoBytesAsInt();
    uint32_t ReadUInt();
    void WriteByte(uint8_t c);
    void WriteShort(int16_t content);
    void WriteInt(int32_t content);
    bool Skip(int32_t skip);
    template <class T>
    void WriteData(T data){
        int nLen = sizeof(data);
        switch (nLen) {
        case 1:
            WriteByte(data);
            break;
        case 2:
            WriteShort(data);
            break;
        case 4:
            WriteInt(data);
        default:
            break;
        }
    }
private:
    std::vector<uint8_t> m_buffer;
    int m_nIndex;
};
#endif // IDATA_H
