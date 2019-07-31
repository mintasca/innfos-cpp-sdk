#ifndef MOTORDATA_H
#define MOTORDATA_H

#include <map>
#include <vector>
#include <list>
#include "actuatordefine.h"



using namespace Actuator;
class  ITimer;

class ActuatorData
{

public:
    enum DATA_STATUS{
        DATA_WAIT,
        DATA_FAILED,
        DATA_NORMAL,
        DATA_NONE,
    };
    explicit ActuatorData(const uint8_t nDeviceId,const uint32_t nDeviceMac,const uint32_t ipAddress);
    uint8_t deviceId() const;
    uint64_t longId()const;
    uint64_t requestDeviceId()const;
    uint32_t deviceMac()const;
    bool isRevisable(Actuator::ActuatorAttribute nDataId)const;
    void setValueByProxy(const int nProxyId,double value);//save value that recive
    void requestAllValue();
    double getValue(Actuator::ActuatorAttribute nDataId)const;
    double getUserRequestValue(Actuator::ActuatorAttribute nDataId)const;
    void regainData(Actuator::ActuatorAttribute nDataId);
    int getDataStatus(Actuator::ActuatorAttribute nDataId)const;
    void requestFailed(Actuator::ActuatorAttribute nDataId);
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
    void enableHeartBeat(bool bEnable);
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
    void readStatus();
    bool waitForACK(const ActuatorAttribute attrId)const;
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
    void setValidValue(const int nProxyId, double value);
    void initTimeOut();
//    void saveDataToFile(std::string fileName);
//    void readDataFromFile(std::string fileName);
//    void readParams(QXmlStreamReader *reader);
private:
private:
    double m_motorData[Actuator::DATA_CNT];
    double m_userRequestData[Actuator::DATA_CNT];
    int m_dataStatus[Actuator::DATA_CNT];
    int m_nHeartFailCnt;//
    ITimer * m_pHeartTimer;
    ITimer * m_pValueTimer;
    ITimer * m_pInitTimer;
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

    double getMotorDataAttrValueAsDouble(const uint64_t longId,const Actuator::ActuatorAttribute attrId)const;
    int32_t getMotorDataAttrValueAsInt(const uint64_t longId,const Actuator::ActuatorAttribute attrId)const;
    void setMotorDataAttrByUser(const uint64_t longId,const Actuator::ActuatorAttribute attrId,double value,bool bSend=true);

    void setMotorDataAttrByProxy(const uint64_t longId,int proxyId,double value);//data from proxy
    void setMotorDataAttrInBatch(const std::list<uint64_t> idList,const Actuator::ActuatorAttribute attrId,double value,bool bSend=true);
    void AddMotorsData(std::multimap<std::pair<uint8_t, uint32_t>, uint32_t> dataMap);
    void clearMotorsData();
    std::vector<uint64_t> getLongIdArray()const;
    std::vector<uint64_t> getLongIdGroup(uint64_t longId)const;
    std::vector<uint16_t> motorErrorHistory(const uint64_t longId)const;
    //bool deviceIdHasExist(uint64_t longId);
//    void activeMotorModeSuccessfully(const uint64_t longId);
    void activeMotorModeInBatch(const std::vector<uint64_t> idList,const Actuator::ActuatorMode mode);
//    void regainAllData(const uint64_t longId);
    void regainData(const uint64_t longId,Actuator::ActuatorAttribute attrId);

    void responseHeart(const uint64_t longId,bool bSuccessfully);

    void switchAutoRequestActual(const uint64_t longId,bool bStart);
    void setAutoRequestInterval(const uint64_t longId,uint32_t mSec);
    uint8_t getOldDeviceId(const uint64_t newLongId);

    void saveAllParams(const uint64_t longId);
    void clearHomingInfo(const uint64_t longId);
    void setHomingOperationMode(const uint64_t longId,const uint8_t nMode);
    void openChartChannel(uint64_t longId, const int nChannelId);
    void closeChartChannel(uint64_t longId, const int nChannelId);
    void switchChartAllChannel(uint64_t longId,bool bOn);
    void switchCalibrationVel(uint64_t longId,uint8_t nValue);
    void switchCalibration(uint64_t longId,uint8_t nValue);
    void startCalibration(uint64_t longId);
    void requestSuccessfully(const uint64_t longId,const uint8_t nProxyId);
    void requestFailed(const uint64_t longId,const uint8_t nProxyId);
    void reconnect(uint64_t longId);
    void clearError(uint64_t longId);
    void enableHeartBeat(uint64_t longId,bool bEnable);

    bool setMotorDataWithACK(const uint64_t longId,const Actuator::ActuatorAttribute attrId,double value,bool bSend=true);
    double regainAttrWithACK(const uint64_t longId,Actuator::ActuatorAttribute attrId,bool *bSuccess=nullptr);

    void sendCmd(uint64_t longId,uint16_t cmdId);
    void sendCmd(uint64_t longId,uint16_t cmdId,uint8_t value);
    void sendCmd(uint64_t longId,uint16_t cmdId,uint32_t value);
    uint64_t getLongIdByDeviceId(uint8_t id)const;
private:
    MotorDataMgr();
//signals:
public:
    void dataChanged(const uint64_t longId,const Actuator::ActuatorAttribute attrId,double value);
    void errorOccured(const uint64_t longId,const uint16_t erroId,std::string errorInfo);
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
    ActuatorData * getMotorDataByLongId(const uint64_t longId)const;
    ActuatorData * getMotorDataByNewId(const uint64_t longId)const;//if device id changed by user, sometimes we need to find the motorData using NewId we set before.
    ActuatorData * getMotorDataByMac(const uint32_t nMac)const;
    bool isUnique(const ActuatorData *pData)const;
//    bool checkIdUnique(std::map<uint8_t,uint32_t> dataMap)const;//check all motors's id si unique or not, if not warning and exit the app
//    void handleUnuiqueError(std::map<uint8_t,uint32_t> dataMap);
    double getMotorDataAttrValue(const uint64_t longId,const Actuator::ActuatorAttribute attrId)const;
    bool waitForACK(const ActuatorData * pData,const ActuatorAttribute attrId)const;
private:
    std::list<ActuatorData *> m_allMotorDatas;
    static MotorDataMgr * m_pMgr;
};


#endif // MOTORDATA_H
