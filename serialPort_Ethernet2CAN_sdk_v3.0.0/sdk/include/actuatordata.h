#ifndef MOTORDATA_H
#define MOTORDATA_H

#include <map>
#include <vector>
#include <list>
#include "actuatordefine.h"
#include "itimer.h"


using namespace Actuator;
//class  ITimer;

class ActuatorData
{

public:
    explicit ActuatorData(const uint8_t nDeviceId,const uint32_t nDeviceMac);
    uint8_t deviceId() const;
    int8_t requestDeviceId()const;
    uint32_t deviceMac()const;
    void setValueByProxy(const int nProxyId,double value);//save value that recive
    void requestAllValue();
    double getValue(Actuator::ActuatorAttribute nDataId)const;
    double getUserRequestValue(Actuator::ActuatorAttribute nDataId)const;
    void setValueByUser(Actuator::ActuatorAttribute nDataId,double value,bool bSendProxy=true);//set value by user, will send proxy or not
    void responseHeart(bool bSuccess);
    bool isOnline()const;
    std::vector<uint16_t> errorHistory()const{
        return m_errorHistory;
    }
    Actuator::ActuatorMode currentMode()const;
    ~ActuatorData();

    bool deviceIdIsAvailable()const;
    void changeDemand(double value);//demand value change
    void switchAutoRequestActual(bool bStart);
    void setAutoRequestInterval(uint32_t mSec);

    void saveAllParams();
    void clearHomingInfo();
    void setHomingOperationMode(const uint8_t nMode);
    void openChartChannel(const int nChannelId);
    void closeChartChannel(const int nChannelId);
    void switchChartAllChannel(bool bOn);
    void switchCalibrationVel(uint8_t nValue);
    void switchCalibration(uint8_t nValue);
    void startCalibration();
    void requestSuccessfully(uint8_t nDataId);
protected:
    void setValue(int nDataId,double value,bool bEmitSignal = true);
    void userRequestValue(int nDataId,double value);
public:
    void initData();
//public slots:
    void activeModeSuccessfully();
    void saveData();
    void loadData();
    void reconnect();

//private slots:
    void requestActualValue();//request value initiatively
private:
//    void saveDataToFile(std::string fileName);
//    void readDataFromFile(std::string fileName);
//    void readParams(QXmlStreamReader *reader);
private:
private:
    double m_motorData[Actuator::DATA_CNT];
    double m_userRequestData[Actuator::DATA_CNT];
    int m_nHeartFailCnt;//
    ITimer * m_pHeartTimer;
    ITimer * m_pValueTimer;
    std::vector<uint16_t> m_errorHistory;

    uint32_t m_nAutoRequestInterval;
};


//motordata manger
#define motorDataMgrInstance MotorDataMgr::getInstance()

class MotorDataMgr
{
public:
    static MotorDataMgr * getInstance();
    virtual ~MotorDataMgr();

    double getMotorDataAttrValueAsDouble(const uint8_t nDeviceId,const Actuator::ActuatorAttribute attrId)const;
    int32_t getMotorDataAttrValueAsInt(const uint8_t nDeviceId,const Actuator::ActuatorAttribute attrId)const;
    void setMotorDataAttrByUser(const uint8_t nDeviceId,const Actuator::ActuatorAttribute attrId,double value,bool bSend=true);
    void setMotorDataAttrByProxy(const uint8_t nDeviceId,int proxyId,double value);//data from proxy
    void setMotorDataAttrInBatch(const std::list<uint8_t> idList,const Actuator::ActuatorAttribute attrId,double value,bool bSend=true);
    void AddMotorsData(std::map<uint8_t,uint32_t> dataMap);
    std::map<uint8_t,uint32_t> getMotorsMap()const;
    std::vector<uint16_t> motorErrorHistory(const uint8_t nDeviceId)const;
    bool deviceIdHasExist(uint8_t nDeviceId);
    void activeMotorModeSuccessfully(const uint8_t nDeviceId);
    void activeMotorModeInBatch(const std::list<uint8_t> idList,const Actuator::ActuatorMode mode);
    void regainAllData(const uint8_t nDeviceId);
    void regainData(const uint8_t nDeviceId,Actuator::ActuatorAttribute attrId);
    void responseHeart(const uint8_t nDeviceId,bool bSuccessfully);

    void switchAutoRequestActual(const uint8_t nDeviceId,bool bStart);
    void setAutoRequestInterval(const uint8_t nDeviceId,uint32_t mSec);
    int8_t getOldDeviceId(const uint8_t nNewDeviceId);

    void saveAllParams(const uint8_t nDeviceId);
    void clearHomingInfo(const uint8_t nDeviceId);
    void setHomingOperationMode(const uint8_t nDeviceId,const uint8_t nMode);
    void openChartChannel(uint8_t nDeviceId, const int nChannelId);
    void closeChartChannel(uint8_t nDeviceId, const int nChannelId);
    void switchChartAllChannel(uint8_t nDeviceId,bool bOn);
    void switchCalibrationVel(uint8_t nDeviceId,uint8_t nValue);
    void switchCalibration(uint8_t nDeviceId,uint8_t nValue);
    void startCalibration(uint8_t nDeviceId);
    void requestSuccessfully(const uint8_t nDeviceId,const uint8_t nProxyId);
    void requestFailed(const uint8_t nDeviceId,const uint8_t nProxyId);
    void reconnect(uint8_t nDeviceId);
    void clearError(uint8_t nDeviceId);
    void sendCmd(uint8_t nDeviceId,uint16_t cmdId);
    void sendCmd(uint8_t nDeviceId,uint16_t cmdId,uint8_t value);
    void sendCmd(uint8_t nDeviceId,uint16_t cmdId,uint32_t value);
private:
    MotorDataMgr();
//signals:
public:
    void dataChanged(const uint8_t nDeviceId,const Actuator::ActuatorAttribute attrId,double value);
    void errorOccured(const uint8_t nDeviceId,const uint16_t erroId,std::string errorInfo);
private:
    class GC{
    public:
        ~GC()
        {
            if(m_pMgr!=nullptr)
            {
                delete m_pMgr;
                m_pMgr = nullptr;
            }
        }
        static GC gc;
    };
private:
    ActuatorData * getMotorDataById(const uint8_t nId)const;
    ActuatorData * getMotorDataByNewId(const uint8_t nId)const;//if device id changed by user, sometimes we need to find the motorData using NewId we set before.
    ActuatorData * getMotorDataByMac(const uint32_t nMac)const;
    bool checkIdUnique(std::map<uint8_t,uint32_t> dataMap)const;//check all motors's id si unique or not, if not warning and exit the app
    void handleUnuiqueError(std::map<uint8_t,uint32_t> dataMap);
    double getMotorDataAttrValue(const uint8_t nDeviceId,const Actuator::ActuatorAttribute attrId)const;
private:
    std::list<ActuatorData *> m_allMotorDatas;
    static MotorDataMgr * m_pMgr;
};


#endif // MOTORDATA_H
