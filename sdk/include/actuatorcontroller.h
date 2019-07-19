#ifndef MOTORSCONTROLLER_H
#define MOTORSCONTROLLER_H

#include "actuatorcontroller_global.h"
#include <functional>
#include <vector>
#include "actuatordefine.h"
#include <iostream>
#include <typeindex>
#include <typeinfo>
//#include "innfosproxy.h"
#include "CSignal.hpp"

#ifdef _MSC_VER
#define DEPRECATE_FUNC(f) __declspec(deprecated("** this is a deprecated function **")) f
#elif defined(__GNUC__)
#define DEPRECATE_FUNC(f) f __attribute__((deprecated("** this is a deprecated function **")))
#else
#pragma message(""WARNING: You need to implement DEPRECATED for this compiler"")
#define DEPRECATE_FUNC(f) f
#endif

using namespace  std;
using namespace Actuator;
#define controllerInst ActuatorController::getInstance()

class ACTUATORCONTROLLERSHARED_EXPORT ActuatorController
{
public:
    ~ActuatorController();
protected:
    ActuatorController();
public:
    /**
     * @brief The UnifiedID struct
     */
    struct UnifiedID
    {
        UnifiedID(uint8_t id,string ip):
            actuatorID(id),
            ipAddress(ip)
        {}
        uint8_t actuatorID=0;//actuator's ID
        string ipAddress;//IP address used by actuator communication
    };


    /**
     * @brief 初始化控制器，使用控制器之前必须先初始化
     * @param 执行器通信方式，默认为以太网通信
     */
    static ActuatorController * initController();

    /**
     * @brief 查找所有已连接的执行器
     * @param ec 错误代码，如果找不到可用执行器，返回对应错误代码
     * @return 返回所有查找到的执行器的UnifiedID
     * @warning 查找执行器需要花费一定时间
    **/
    std::vector<UnifiedID> lookupActuators(Actuator::ErrorsDefine & ec);

    /**
     * @brief 获取控制器对象
     * @return 控制器对象
     */
    static ActuatorController * getInstance();
    /**
     * @brief 处理控制器事件
     */
    static void processEvents();

    /**
 * @brief 是否有可用执行器
**/
    bool hasAvailableActuator()const;
/**
 * @brief 获取所有执行器Id数组
 * @return id数组
**/
    vector<uint8_t> getActuatorIdArray()const;
    /**
     * @brief 获取所有执行器UnifiedID数组
     * @return UnifiedID数组
    **/
    vector<UnifiedID> getActuatorUnifiedIDArray()const;

    /**
     * @brief getUnifiedIDGroup getUnifiedIDGroup 获取指定ip地址下所有执行器的UnifiedID
     * @param ipAddress ip地址的字符串
     * @return 返回同一通信单元内的所有执行器ID
     */
    vector<uint8_t>  getUnifiedIDGroup(const string& ipAddress);

/**
 * @brief 启动所有执行器，若执行器未启动，需先调用此函数，调用后需要等待3秒后才能操作
**/
    void launchAllActuators();
    /**
 * @brief 关闭所有执行器
 * @date 2018/01/15
**/
    void closeAllActuators();
/**
 * @brief 启动指定执行器
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
 * @warning
**/
    bool launchActuator(uint8_t id,const string & ipAddress="");
    /**
     * @brief 启动指定执行器
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     * @warning 执行器启动成功返回true,否则返回false
    **/
    bool launchActuatorInBatch(const vector<UnifiedID>& actuators);
    /**
 * @brief 关闭指定执行器
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
**/
    void closeActuator(uint8_t id,const string & ipAddress="");

    /**
     * @brief activateActuatorMode 激活单个执行器的指定模式
     * @param id 执行器id
     * @param nMode 指定的激活模式
     * * @param ipAddress 目标ip地址字符串
     */
    void activateActuatorMode(uint8_t id, const Actuator::ActuatorMode nMode,const string&ipAddress="");

/**
 * @brief 激活执行器的指定模式
 * @param idArray 执行器id数组
 * @param nMode 要激活的模式
**/
    void activateActuatorModeInBantch(vector<uint8_t> idArray, const Actuator::ActuatorMode nMode);
    /**
     * @brief 激活执行器的指定模式
     * @param idArray 执行器UnifiedID数组
     * @param nMode 要激活的模式
    **/
    void activateActuatorModeInBantch(vector<UnifiedID> UnifiedIDArray, const Actuator::ActuatorMode nMode);
/**
 * @brief 开启或关闭执行器自动刷新功能，自动请求执行器电流、速度、位置、电压、温度、逆变器温度（默认关闭此功能）
 * @param id 执行器id
 * @param bOpen 是否开启
 * @param ipAddress 目标ip地址字符串
**/
    void switchAutoRefresh(uint8_t id,bool bOpen,const string & ipAddress="");


    /**
 * @brief 设置自动刷新时间间隔（默认时间间隔为1s）
 * @param id 执行器id
 * @param mSec 毫秒数
 * @param ipAddress 目标ip地址字符串
**/
    void setAutoRefreshInterval(uint8_t id, uint32_t mSec,const string & ipAddress="");

    //position loop
    /**
 * @brief 设置位置
 * @param id 执行器id
 * @param pos 目标位置，单位是圈数
 * @param ipAddress 目标ip地址字符串
**/
    void setPosition(uint8_t id,double pos,const string & ipAddress="");

    /**
 * @brief 获取当前位置
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次位置读取,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前位置，单位是转数
**/
    double getPosition(uint8_t id,bool bRefresh,const string & ipAddress="")const;

    /**
     * @brief setPositionKp 设置位置环的比例
     * @param id 执行器id
     * @param Kp 位置环的比例
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionKp(uint8_t id,double Kp,const string & ipAddress="");
    double getPostionKp(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setPositionKi(uint8_t id,double Ki,const string & ipAddress="");
    double getPostionKi(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setPositionKd(uint8_t id,double Kd,const string & ipAddress="");
    double getPostionKd(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setPositionUmax(uint8_t id,double max,const string & ipAddress="");
    double getPostionUmax(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setPositionUmin(uint8_t id,double min,const string & ipAddress="");
    double getPostionUmin(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setPositionOffset(uint8_t id, double offset, const string & ipAddress="");
    double getPostionOffset(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setMaximumPosition(uint8_t id,double maxPos,const string & ipAddress="");
    double getMaximumPostion(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setMinimumPosition(uint8_t id,double minPos,const string & ipAddress="");
    double getMinimumPostion(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void enablePositionLimit(uint8_t id, bool enable, const string & ipAddress="");
    bool isPostionLimitEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setHomingPosition(uint8_t id,double homingPos,const string & ipAddress="");
    void enablePositionFilter(uint8_t id,bool enable,const string & ipAddress="");
    bool isPositionFilterEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    double getPositionCutoffFrequency(uint8_t id, bool bRefresh, const string &ipAddress="") const;
    void setPositionCutoffFrequency(uint8_t id, double frequency, const string &ipAddress="");
    /**
 * @brief 清除homing信息，包括左右极限和0位
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
**/
    void clearHomingInfo(uint8_t id,const string & ipAddress="");
    //profile position
    void setProfilePosAcceleration(uint8_t id, double acceleration, const string & ipAddress="");
    double getProfilePosAcceleration(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setProfilePosDeceleration(uint8_t id, double deceleration, const string & ipAddress="");
    double getProfilePosDeceleration(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setProfilePosMaxVelocity(uint8_t id, double maxVelocity, const string & ipAddress="");
    double getProfilePosMaxVelocity(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    //velocity loop
    /**
 * @brief 设置速度
 * @param id 执行器id
 * @param vel 目标速度，单位是转/每分钟
 * @param ipAddress 目标ip地址字符串
**/
    void setVelocity(uint8_t id,double vel,const string & ipAddress="");
    /**
 * @brief 获取当前速度
 * @param id 执行器id
 * @param bRefresh是否需要刷新,如果为true，会自动请求一次速度读取,并等待返回，如果为false,则会立即返回最近一次请求速度返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前速度，单位是转/每分钟
**/
    double getVelocity(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 获取速度环比例
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次速度环比例读取,并等待返回，如果为false,则会立即返回最近一次请求速度环比例返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前速度环比例
**/
    double getVelocityKp(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 设置速度环比例
 * @param id 执行器id
 * @param 比例
 * @param ipAddress 目标ip地址字符串
 * @return 设置成功或失败
**/
    void setVelocityKp(uint8_t id,double Kp,const string & ipAddress="");

    /**
 * @brief 获取速度环积分
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次速度环积分读取,并等待返回，如果为false,则会立即返回最近一次请求速度环积分返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前速度环积分
**/
    double getVelocityKi(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief setVelocityKi 设置速度环积分
     * @param id 执行器id
     * @param Ki 积分
     * @param ipAddress 目标ip地址字符串
     * @return 设置成功或失败
     */
    void setVelocityKi(uint8_t id, double Ki, const string &ipAddress="");

    /**
 * @brief 获取速度环最大输出限幅
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次速度环最大输出限幅读取,并等待返回，如果为false,则会立即返回最近一次请求速度环最大输出限幅返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 最大输出限幅
**/
    double getVelocityUmax(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置速度环最大输出限幅
     * @param id 执行器id
     * @param max 最大输出限幅
     * @param ipAddress 目标ip地址字符串
     * @return 设置成功或失败
     */
    void setVelocityUmax(uint8_t id, double max, const string &ipAddress="");

    /**
 * @brief 获取速度环最小输出限幅
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次速度环最小输出限幅读取,并等待返回，如果为false,则会立即返回最近一次请求速度环最小输出限幅返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 最小输出限幅
**/
    double getVelocityUmin(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置速度环最小输出限幅
     * @param id 执行器id
     * @param min 最小输出限幅
     * @param ipAddress 目标ip地址字符串
     * @return 设置成功或失败
     */
    void setVelocityUmin(uint8_t id, double min, const string &ipAddress="");
    double getVelocityRange(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void enableVelocityFilter(uint8_t id,bool enable,const string & ipAddress="");
    bool isVelocityFilterEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    double getVelocityCutoffFrequency(uint8_t id, bool bRefresh, const string &ipAddress="") const;
    void setVelocityCutoffFrequency(uint8_t id, double frequency, const string &ipAddress="");
    void setVelocityLimit(uint8_t id,double limit,const string & ipAddress="");
    double getVelocityLimit(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    //profile velocity
    void setProfileVelocityAcceleration(uint8_t id,double minPos,const string & ipAddress="");
    double getProfileVelocityAcceleration(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setProfileVelocityDeceleration(uint8_t id,double minPos,const string & ipAddress="");
    double getProfileVelocityDeceleration(uint8_t id,bool bRefresh,const string & ipAddress="")const;

    //current loop
    /**
 * @brief 设置电流
 * @param id 执行器id
 * @param current 目标电流，单位是A
 * @param ipAddress 目标ip地址字符串
**/
    void setCurrent(uint8_t id,double current,const string & ipAddress="");

    /**
 * @brief 获取当前电流
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次电流读取,并等待返回，如果为false,则会立即返回最近一次请求电流返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前电流，单位是A
**/
    double getCurrent(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 获取电流环比例
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次电流环比例读取,并等待返回，如果为false,则会立即返回最近一次请求电流环比例返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前电流环比例
**/
    double getCurrentKp(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 获取电流环积分
 * @param id 执行器id
 * @param bRefresh是否需要刷新，如果为true，会自动请求一次电流环积分读取,并等待返回，如果为false,则会立即返回最近一次请求电流环积分返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前电流环积分
**/
    double getCurrentKi(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    double getCurrentRange(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void enableCurrentFilter(uint8_t id,bool enable,const string & ipAddress="");
    bool isCurrentFilterEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    double getCurrentCutoffFrequency(uint8_t id, bool bRefresh, const string &ipAddress="") const;
    void setCurrentCutoffFrequency(uint8_t id, double frequency, const string &ipAddress="");
    void setCurrentLimit(uint8_t id,double limit,const string & ipAddress="");
    double getCurrentLimit(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 执行器保存当前所有参数
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
**/
    void saveAllParams(uint8_t id,const string & ipAddress="");

    /**
     * @brief setMinPosLimit 设置最小位置限制，值为当前电机位置
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     */
    void setMinPosLimit(uint8_t id,const string & ipAddress="");

    /**
     * @brief setMinPosLimit 设置最小位置限制
     * @param id 执行器id
     * @param posValue 最小位置限制
     * @param ipAddress 目标ip地址字符串
     */
    void setMinPosLimit(uint8_t id,double posValue,const string & ipAddress="");

    //chart info
    void setChartFrequency(uint8_t id, double frequency, const string & ipAddress="");
    double getChartFrequency(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setChartThreshold(uint8_t id, double threshold, const string & ipAddress="");
    double getChartThreshold(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void enableChart(uint8_t id,bool enable,const string & ipAddress="");
    bool isChartEnable(uint8_t id,bool bRefresh,const string & ipAddress="");
    /**
 * @brief 开启图表指定通道
 * @param id 执行器id
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
**/
    void openChartChannel(uint8_t id,uint8_t  nChannelId,const string & ipAddress="");

    /**
 * @brief 关闭图表指定通道
 * @param id 执行器id
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
**/
    void closeChartChannel(uint8_t id, uint8_t nChannelId,const string & ipAddress="");


    /**
     * @brief setCurrentChartMode 设置电流模式图显模式，有Actuator::IQ_CHART和Actuator::ID_CHART两种模式
     * @param id 执行器id
     * @param mode 图显模式
     */
    void setCurrentChartMode(uint8_t id, uint8_t mode);
    /**
     * @brief setCurrentChartMode 设置电流模式图显模式，有Actuator::IQ_CHART和Actuator::ID_CHART两种模式
     * @param longId 执行器长id
     * @param mode 图显模式
     */
    void setCurrentChartMode(uint64_t longId, uint8_t mode);

    //other info
    double getVoltage(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    double getLockEnergy(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setLockEnergy(uint8_t id,double energy,const string & ipAddress="");
    double getMotorTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    double getInverterTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    double getMotorProtectedTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setMotorProtectedTemperature(uint8_t id,double temp,const string & ipAddress="");
    double getMotorRecoveryTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setMotorRecoveryTemperature(uint8_t id,double temp,const string & ipAddress="");
    double getInverterProtectedTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setInverterProtectedTemperature(uint8_t id,double temp,const string & ipAddress="");
    double getInverterRecoveryTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    void setInverterRecoveryTemperature(uint8_t id,double temp,const string & ipAddress="");
    bool isOnline(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    bool setActuatorID(uint8_t currentID, uint8_t newID, const string & ipAddress="");
    uint32_t getActuatorSerialNumber(uint8_t id,const string & ipAddress="")const;
    Actuator::ActuatorMode getActuatorMode(uint8_t id,const string & ipAddress="")const;
    uint32_t getErrorCode(uint8_t id,const string & ipAddress="")const;



    /**
 * @brief 重新获取属性,将请求刷新属性
 * @param id 执行器id
 * @param attrId 执行器属性Id
**/
    void regainAttrbute(uint8_t id,uint8_t attrId,const string & ipAddress="");

    /**
 * @brief 获取执行器错误历史记录
 * @param id 执行器id
**/
    vector<uint16_t> getErrorHistory(uint8_t id,const string & ipAddress="");

    /**
 * @brief 执行器掉线重连
 * @param id 执行器id
**/
    void reconnect(uint8_t id,const string & ipAddress="");

    /**
 * @brief 执行器错误清除
 * @param id 执行器id
**/
    void clearError(uint8_t id,const string & ipAddress="");

    /**
     * @brief versionString 获取sdk版本号字符串
     * @return sdk版本号字符串
     */
    string versionString()const;


    /**
     * @brief getCVPValue 获取电流速度位置的值(如果同时需要三个值，该接口效率比较高）
     * @param id 执行器id
     */
    void getCVPValue(uint8_t id,const string & ipAddress="");


    using doubleFuncPointer = void(*)(UnifiedID, uint8_t ,double);
    using stringFuncPointer = void(*)(UnifiedID, uint16_t ,string);
    using doubleFunction = function<void(UnifiedID, uint8_t ,double)>;
    using stringFunction = function<void(UnifiedID, uint16_t ,string)>;


    void addParamChangeCallback(doubleFuncPointer callback);
    void addParamChangeCallback(doubleFunction callback);
    void addErrorCallback(stringFuncPointer callback);
    void addErrorCallback(stringFunction callback);

    /**
     * @brief clearCallbackHandlers 清除所有回调
     */
    void clearCallbackHandlers();

#ifdef IMU_ENABLE
    /**
     * @brief requestAllQuaternions 请求全身惯导模块数据
     * @param longId 惯导模块接收板id
     */
    void requestAllQuaternions(uint64_t longId=0);
    /**
     * @brief requestSingleQuaternion 请求单个惯导模块
     * @param longId 惯导模块id
     */
    void requestSingleQuaternion(uint64_t longId=0);
//    void requestQuaternion(const std::string & target);
    /**
     * @brief receiveQuaternion 接收惯导模块数据
     * @param nIMUId 惯导模块id
     * @param w
     * @param x
     * @param y
     * @param z
     */
    void receiveQuaternion(uint64_t nIMUId,double w,double x,double y,double z);
    void requestLossRatio();
    void receiveLossRatio(uint64_t nIMUId,uint32_t receive,uint32_t lost);
#endif

private:
    //v3.0 add end
    void switchCalibrationVel(uint64_t longId,uint8_t nValue);
    void switchCalibration(uint64_t longId,uint8_t nValue);
    void startCalibration(uint64_t longId);
    void sendCmd(uint64_t longId,uint16_t cmdId,uint32_t value);
    bool setActuatorAttributeWithACK(uint64_t longId, ActuatorAttribute attrId, double value);
    void clearError(uint64_t longId);
    void reconnect(uint64_t longId);
    vector<uint16_t> getErrorHistory(uint64_t longId);
    void regainAttrbute(uint64_t longId,uint8_t attrId);
    void switchChartAllChannel(uint64_t longId,bool bOn);
    void closeChartChannel(uint64_t longId, uint8_t nChannelId);
    void openChartChannel(uint64_t longId,uint8_t  nChannelId);
    void setMinPosLimit(uint64_t longId,double posValue);
    void setMinPosLimit(uint64_t longId);
    void setHomingOperationMode(uint8_t id,uint8_t nMode);
    void setHomingOperationMode(uint64_t longId,uint8_t nMode);
    void clearHomingInfo(uint64_t longId);
    void saveAllParams(uint64_t id);
    void setActuatorAttribute(uint8_t id,Actuator::ActuatorAttribute attrId,double value,const string & ipAddress="");
    void setActuatorAttribute(uint64_t longId,Actuator::ActuatorAttribute attrId,double value);
    double getActuatorAttribute(uint8_t id,Actuator::ActuatorAttribute attrId,const string & ipAddress="")const;
    double getActuatorAttribute(uint64_t longId,Actuator::ActuatorAttribute attrId)const;
    double getCurrent(uint64_t longId,bool bRefresh=false)const;
    double getVelocity(uint64_t longId,bool bRefresh=false)const;
    double getPosition(uint64_t longId,bool bRefresh=false)const;
    void setCurrent(uint64_t longId,double current) ;
    void setVelocity(uint64_t longId,double vel);
    void setPosition(uint64_t longId, double pos);
    void setAutoRefreshInterval(uint64_t longId, uint32_t mSec);
    void switchAutoRefresh(uint64_t longId, bool bOpen);
    //void activateActuatorMode(uint64_t id, const Actuator::ActuatorMode nMode);
    vector<uint64_t> getActuatorIdGroup(uint64_t longId)const;
    void autoRecoginze();
    void finishRecognizeCallback();
    void onRequestCallback(uint64_t longId, uint8_t nProxyId,double value);
    void errorOccur(uint64_t longId,uint16_t errorId, std::string errorStr);
    void motorAttrChanged(uint64_t longId,uint8_t attrId,double value);
    void startNewChart();
    void chartValueChange(uint8_t channelId,double value);
    void startLog();
    void stopLog();
    uint32_t ipToUint(const string & ipAddress);
    /**
     * @brief 将ip地址字符串和id转换成长id
     * @param ipAddress 目标ip地址字符串
     * @param id id
     * @return 对应长id
     * @warning 该接口仅支持以太网的通信方式
     */
    static uint64_t toLongId(uint8_t id,const string &ipAddress="");
    /**
     * @brief 将长id转换成ip地址字符串
     * @param longId 长id
     * @return ip地址字符串
     * @warning 该接口仅支持以太网的通信方式
     */
    static std::string toString(uint64_t longId);
    /**
     * @brief 长id转换成id
     * @param longId 长id
     * @return id
     */
    static uint8_t toByteId(uint64_t longId);
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
    int m_nLaunchMotorsCnt;
public:
    //signals
    /**
     * @brief m_sOperationFinished 操作完成信号 <uint8_t 执行器id,uint8_t 操作类型，类型枚举OperationFlags>
     */
    CSignal<uint8_t,uint8_t> *m_sOperationFinished;
    CSignal<uint64_t,uint8_t> *m_sOperationFinishedL;
    /**
     * @brief m_sRequestBack 请求返回信号 <uint8_t 执行器id ,uint8_t 请求协议id,double 返回值>
     */
    CSignal<uint8_t,uint8_t,double> *m_sRequestBack;
    CSignal<uint64_t,uint8_t,double> *m_sRequestBackL;
    /**
     * @brief m_sError 错误信号 <uint8_t 执行器id,uint16_t 错误id,std::string 错误信息>
     */
    CSignal<uint8_t,uint16_t,std::string> *m_sError;
    CSignal<uint64_t,uint16_t,std::string> *m_sErrorL;
    /**
     * @brief m_sActuatorAttrChanged 电机数据变化，自动刷新或者主动刷新返回以后，会触发该信号
     * <uint8_t 执行器id,uint8_t 变化属性id,double 属性值>
     */
    CSignal<uint8_t,uint8_t,double> *m_sActuatorAttrChanged;
    CSignal<uint64_t,uint8_t,double> *m_sActuatorAttrChangedL;
    /**
     * @brief m_sNewChartStart 图显信号
     */
    CSignal<> *m_sNewChartStart;
    /**
     * @brief m_sChartValueChange 图显数据
     */
    CSignal<uint8_t,double> *m_sChartValueChange;
#ifdef IMU_ENABLE
    CSignal<uint8_t,double,double,double,double> *m_sQuaternion;
    CSignal<uint64_t,double,double,double,double> *m_sQuaternionL;
    CSignal<uint8_t,uint32_t,uint32_t> * m_sLossRatio;
#endif

private:
    static ActuatorController * m_pInstance;
    std::vector<int> *m_lConnectionIds;
    bool m_bInitFinished;
    Actuator::ErrorsDefine _errorCode;
};

#endif // MOTORSCONTROLLER_H
