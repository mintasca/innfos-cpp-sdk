#ifndef MOTORSCONTROLLER_H
#define MOTORSCONTROLLER_H

#include "actuatorcontroller_global.h"
#include <functional>
#include <vector>
#include "actuatordefine.h"
#include <iostream>
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
     * @brief 短id转换成长id
     * @param id 短id
     * @return 对应长id
     * @warning 如果不同通信单元存在相同短id的执行器，将随机返回一个执行器的长id
     */
    static uint64_t toLongId(uint8_t id);
    /**
     * @brief 将ip地址字符串和短id转换成长id
     * @param ipStr 目标ip地址字符串
     * @param id 短id
     * @return 对应长id
     * @warning 该接口仅支持以太网的通信方式
     */
    static uint64_t toLongId(std::string ipStr,uint8_t id);
    /**
     * @brief 将长id转换成ip地址字符串
     * @param longId 长id
     * @return ip地址字符串
     * @warning 该接口仅支持以太网的通信方式
     */
    static std::string toString(uint64_t longId);
    /**
     * @brief 长id转换成短id
     * @param longId 长id
     * @return 短id
     */
    static uint8_t toByteId(uint64_t longId);
    /**
     * @brief 初始化控制器，使用控制器之前必须先初始化
     * @param 执行器通信方式，默认为以太网通信
     */
    static void initController(int nCommunicationType=Actuator::Via_Ethernet);
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
 * @brief 识别所有可用执行器
**/
    void autoRecoginze();
    /**
 * @brief 是否有可用执行器
**/
    bool hasAvailableActuator()const;
/**
 * @brief 获取所有执行器Id数组
 * @return 短id数组
**/
    vector<uint8_t> getActuatorIdArray()const;
    /**
     * @brief 获取所有执行器Id数组
     * @return 长id数组
    **/
    vector<uint64_t> getActuatorLongIdArray()const;
    /**
     * @brief 获取和指定longId的执行器处于同一通信单元的所有执行器longId
     * @param[in] 指定执行器longId
     * @return 返回同一通信单元内的所有执行器longId
     */
    vector<uint64_t> getActuatorIdGroup(uint64_t longId)const;
    /**
     * @brief getActuatorIdGroup getActuatorIdGroup 获取名字为name的通信单元连接的所有执行器longId
     * @param name 通信单元名称（串口为串口名字如COM3等,以太网为ip地址的字符串）
     * @return 返回同一通信单元内的所有执行器longId
     */
    vector<uint64_t> getActuatorIdGroup(std::string name);
/**
 * @brief 激活执行器的指定模式
 * @param idArray 执行器短id数组
 * @param nMode 要激活的模式
**/
    void activateActuatorMode(vector<uint8_t> idArray, const Actuator::ActuatorMode nMode);
    /**
     * @brief 激活执行器的指定模式
     * @param idArray 执行器长id数组
     * @param nMode 要激活的模式
    **/
    void activateActuatorMode(vector<uint64_t> longIdArray, const Actuator::ActuatorMode nMode);
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
 * @param id 执行器短id
 * @warning 如果当前执行器处于关机状态，需等待Actuator::Launch_Finished信号发出以后才能操作，如果当前执行器处于开机状态，则不会触发Actuator::Launch_Finished信号
**/
    void launchActuator(uint8_t id);
    /**
     * @brief 启动指定执行器
     * @param longId 执行器长id
     * @warning 如果当前执行器处于关机状态，需等待Actuator::Launch_Finished信号发出以后才能操作，如果当前执行器处于开机状态，则不会触发Actuator::Launch_Finished信号
    **/
    void launchActuator(uint64_t longId);
    /**
 * @brief 关闭指定执行器
 * @param id 执行器短id
**/
    void closeActuator(uint8_t id);
    /**
 * @brief 关闭指定执行器
 * @param longId 执行器长id
**/
    void closeActuator(uint64_t longId);
    /**
 * @brief 开启或关闭执行器自动刷新功能，自动请求执行器电流、速度、位置、电压、温度、逆变器温度（默认关闭此功能）
 * @param id 执行器短id
 * @param bOpen 是否开启
**/
    void switchAutoRefresh(uint8_t id,bool bOpen);
    /**
 * @brief 开启或关闭执行器自动刷新功能，自动请求执行器电流、速度、位置、电压、温度、逆变器温度（默认关闭此功能）
 * @param longId 执行器长id
 * @param bOpen 是否开启
**/
    void switchAutoRefresh(uint64_t longId, bool bOpen);
    /**
 * @brief 设置自动刷新时间间隔（默认时间间隔为1s）
 * @param id 执行器短id
 * @param mSec 毫秒数
**/
    void setAutoRefreshInterval(uint8_t id, uint32_t mSec);
    /**
 * @brief 设置自动刷新时间间隔（默认时间间隔为1s）
 * @param longId 执行器长id
 * @param mSec 毫秒数
**/
    void setAutoRefreshInterval(uint64_t longId, uint32_t mSec);
    /**
 * @brief 设置位置
 * @param id 执行器短id
 * @param pos 目标位置，单位是圈数
**/
    void setPosition(uint8_t id,double pos);
    /**
 * @brief 设置位置
 * @param longId 执行器长id
 * @param pos 目标位置，单位是圈数
**/
    void setPosition(uint64_t longId, double pos);
    /**
 * @brief 设置速度
 * @param id 执行器短id
 * @param vel 目标速度，单位是转/每分钟
**/
    void setVelocity(uint8_t id,double vel);
    /**
 * @brief 设置速度
 * @param longId 执行器长Id
 * @param vel 目标速度，单位是转/每分钟
**/
    void setVelocity(uint64_t longId,double vel);
    /**
 * @brief 设置电流
 * @param id 执行器短id
 * @param current 目标电流，单位是A
**/
    void setCurrent(uint8_t id,double current);
    /**
 * @brief 设置电流
 * @param longId 执行器长Id
 * @param current 目标电流，单位是A
**/
    void setCurrent(uint64_t longId,double current);
    /**
 * @brief 获取当前位置
 * @param id 执行器短id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次位置读取
 * @return 当前位置，单位是转数
**/
    double getPosition(uint8_t id,bool bRefresh=false)const;
    double getPosition(uint64_t longId,bool bRefresh=false)const;
    /**
 * @brief 获取当前速度
 * @param id 执行器短id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次速度读取
 * @return 当前速度，单位是转/每分钟
**/
    double getVelocity(uint8_t id,bool bRefresh=false)const;
    /**
 * @brief 获取当前速度
 * @param longId 执行器长id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次速度读取
 * @return 当前速度，单位是转/每分钟
**/
    double getVelocity(uint64_t longId,bool bRefresh=false)const;
    /**
 * @brief 获取当前电流
 * @param id 执行器短id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次电流读取
 * @return 当前电流，单位是A
**/
    double getCurrent(uint8_t id,bool bRefresh=false)const;
    /**
 * @brief 获取当前电流
 * @param longId 执行器长id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次电流读取
 * @return 当前电流，单位是A
**/
    double getCurrent(uint64_t longId,bool bRefresh=false)const;
    /**
 * @brief 设置电机属性
 * @param id 执行器短id
 * @param attrId 属性Id，参见Actuator::ActuatorAttribute
 * @param value 属性设置值
**/
    void setActuatorAttribute(uint8_t id,Actuator::ActuatorAttribute attrId,double value);
    /**
 * @brief 设置电机属性
 * @param longId 执行器长id
 * @param attrId 属性Id，参见Actuator::ActuatorAttribute
 * @param value 属性设置值
**/
    void setActuatorAttribute(uint64_t longId,Actuator::ActuatorAttribute attrId,double value);
    /**
 * @brief 获取电机属性值
 * @param id 执行器短id
 * @param attrId 属性Id，参见Actuator::ActuatorAttribute
 * @return 返回属性值
**/
    double getActuatorAttribute(uint8_t id,Actuator::ActuatorAttribute attrId)const;
    /**
 * @brief 获取电机属性值
 * @param longId 执行器长id
 * @param attrId 属性Id，参见Actuator::ActuatorAttribute
 * @return 返回属性值
**/
    double getActuatorAttribute(uint64_t longId,Actuator::ActuatorAttribute attrId)const;
    /**
 * @brief 执行器保存当前所有参数
 * @param id 执行器短id
**/
    void saveAllParams(uint8_t id);
    /**
 * @brief 执行器保存当前所有参数
 * @param id 执行器长id
**/
    void saveAllParams(uint64_t id);
    /**
 * @brief 清除homing信息，包括左右极限和0位
 * @param id 执行器短id
**/
    void clearHomingInfo(uint8_t id);
    /**
 * @brief 清除homing信息，包括左右极限和0位
 * @param longId 执行器长id
**/
    void clearHomingInfo(uint64_t longId);
    /**
 * @brief 设置homing操作模式
 * @param id 执行器短id
 * @param nMode 操作模式（Actuator::Homing_Auto,Actuator::Homing_Manual）
**/
    void setHomingOperationMode(uint8_t id,uint8_t nMode);
    /**
 * @brief 设置homing操作模式
 * @param longId 执行器长id
 * @param nMode 操作模式（Actuator::Homing_Auto,Actuator::Homing_Manual）
**/
    void setHomingOperationMode(uint64_t longId,uint8_t nMode);
    /**
     * @brief setMinPosLimit 设置最小位置限制，值为当前电机位置
     * @param id 执行器短id
     */
    void setMinPosLimit(uint8_t id);
    /**
     * @brief setMinPosLimit 设置最小位置限制，值为当前电机位置
     * @param longId 执行器长id
     */
    void setMinPosLimit(uint64_t longId);
    /**
     * @brief setMinPosLimit 设置最小位置限制
     * @param id 执行器短id
     * @param posValue 最小位置限制
     */
    void setMinPosLimit(uint8_t id,double posValue);
    /**
     * @brief setMinPosLimit 设置最小位置限制
     * @param longId 执行器长id
     * @param posValue 最小位置限制
     */
    void setMinPosLimit(uint64_t longId,double posValue);
    /**
     * @brief setMaxPosLimit 设置最大位置限制，值为当前电机位置，最大位置限制必须大于最小位置限制
     * @param id 执行器短id
     */
    void setMaxPosLimit(uint8_t id);
    /**
     * @brief setMaxPosLimit 设置最大位置限制，值为当前电机位置，最大位置限制必须大于最小位置限制
     * @param longId 执行器长id
     */
    void setMaxPosLimit(uint64_t longId);
    /**
     * @brief setMinPosLimit 设置最大位置限制
     * @param id 执行器短id
     * @param posValue 最大位置限制
     */
    void setMaxPosLimit(uint8_t id,double posValue);
    /**
     * @brief setMinPosLimit 设置最大位置限制
     * @param longId 执行器长id
     * @param posValue 最大位置限制
     */
    void setMaxPosLimit(uint64_t longId,double posValue);

    /**
     * @brief setHomingPosition 将给定位置设置为0位，最大最小位置会相应变化
     * @param id 执行器短id
     * @param posValue 将要变为0位的位置
     */
    void setHomingPosition(uint8_t id,double posValue);
    /**
     * @brief setHomingPosition 将给定位置设置为0位，最大最小位置会相应变化
     * @param longId 执行器长id
     * @param posValue 将要变为0位的位置
     */
    void setHomingPosition(uint64_t longId,double posValue);

    /**
 * @brief 开启图表指定通道
 * @param id 执行器短id
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
**/
    void openChartChannel(uint8_t id,uint8_t  nChannelId);
    /**
 * @brief 开启图表指定通道
 * @param longId 执行器长id
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
**/
    void openChartChannel(uint64_t longId,uint8_t  nChannelId);
    /**
 * @brief 关闭图表指定通道
 * @param id 执行器短id
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
**/
    void closeChartChannel(uint8_t id, uint8_t nChannelId);
    /**
 * @brief 关闭图表指定通道
 * @param longId 执行器长id
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
**/
    void closeChartChannel(uint64_t longId, uint8_t nChannelId);
    /**
 * @brief 开启或关闭图表所有通道
 * @param id 执行器短id
 * @param bOn 是否开启
**/
    void switchChartAllChannel(uint8_t id,bool bOn);
    /**
 * @brief 开启或关闭图表所有通道
 * @param longId 执行器长id
 * @param bOn 是否开启
**/
    void switchChartAllChannel(uint64_t longId,bool bOn);

    /**
     * @brief setCurrentChartMode 设置电流模式图显模式，有Actuator::IQ_CHART和Actuator::ID_CHART两种模式
     * @param id 执行器短id
     * @param mode 图显模式
     */
    void setCurrentChartMode(uint8_t id, uint8_t mode);
    /**
     * @brief setCurrentChartMode 设置电流模式图显模式，有Actuator::IQ_CHART和Actuator::ID_CHART两种模式
     * @param longId 执行器长id
     * @param mode 图显模式
     */
    void setCurrentChartMode(uint64_t longId, uint8_t mode);
    /**
 * @brief 重新获取属性,将请求刷新属性
 * @param id 执行器短id
 * @param attrId 执行器属性Id
**/
    void regainAttrbute(uint8_t id,uint8_t attrId);
    /**
 * @brief 重新获取属性,将请求刷新属性
 * @param longId 执行器长id
 * @param attrId 执行器属性Id
**/
    void regainAttrbute(uint64_t longId,uint8_t attrId);
    /**
 * @brief 获取执行器错误历史记录
 * @param id 执行器短id
**/
    vector<uint16_t> getErrorHistory(uint8_t id);
    /**
 * @brief 获取执行器错误历史记录
 * @param longId 执行器长id
**/
    vector<uint16_t> getErrorHistory(uint64_t longId);
    /**
 * @brief 执行器掉线重连
 * @param id 执行器短id
**/
    void reconnect(uint8_t id);
    /**
 * @brief 执行器掉线重连
 * @param longId 执行器长id
**/
    void reconnect(uint64_t longId);
    /**
 * @brief 执行器错误清除
 * @param id 执行器短id
**/
    void clearError(uint8_t id);
    /**
 * @brief 执行器错误清除
 * @param longId 执行器长id
**/
    void clearError(uint64_t longId);
    /**
     * @brief versionString 获取sdk版本号字符串
     * @return sdk版本号字符串
     */
    string versionString()const;
    //v3.0 add begin
    /**
     * @brief getMaxCurrent,获取执行器的最大允许电流，单位为A，该值仅与执行器型号有关，并且不可被修改
     * @param id 执行器短id
     * @return 执行器的最大允许电流，单位为A
     */
    double getMaxCurrent(uint8_t id)const;
    /**
     * @brief getMaxCurrent,获取执行器的最大允许电流，单位为A，该值仅与执行器型号有关，并且不可被修改
     * @param longId 执行器长id
     * @return 执行器的最大允许电流，单位为A
     */
    double getMaxCurrent(uint64_t longId)const;

    /**
     * @brief getMaxVelocity,获取执行器的最大允许速度，单位为RPM，该值仅与执行器型号有关，并且不可被修改
     * @param id 执行器短id
     * @return 执行器的最大允许速度，单位为RPM
     */
    double getMaxVelocity(uint8_t id)const;
    /**
     * @brief getMaxVelocity,获取执行器的最大允许速度，单位为RPM，该值仅与执行器型号有关，并且不可被修改
     * @param longId 执行器长id
     * @return 执行器的最大允许速度，单位为RPM
     */
    double getMaxVelocity(uint64_t longId)const;
    /**
     * @brief getMaxOutputCurrent 获取执行器最大输出电流，单位为A，该值不会大于执行器最大允许电流
     * @param id 执行器短id
     * @return 执行器最大输出电流，单位为A
     */
    double getMaxOutputCurrent(uint8_t id)const;
    /**
     * @brief getMaxOutputCurrent 获取执行器最大输出电流，单位为A，该值不会大于执行器最大允许电流
     * @param longId 执行器长id
     * @return 执行器最大输出电流，单位为A
     */
    double getMaxOutputCurrent(uint64_t longId)const;
    /**
     * @brief setMaxOutputCurrent 设置执行器最大输出电流，单位为A，该值不会大于执行器最大允许电流且必须大于最小输出电流
     * @param id 执行器短id
     * @param maxOutputCurrent 设置的最大输出电流
     * @return 如果设置的电流值有效，返回true，否则返回false
     */
    bool setMaxOutputCurrent(uint8_t id,double maxOutputCurrent);
    /**
     * @brief setMaxOutputCurrent 设置执行器最大输出电流，单位为A，该值不会大于执行器最大允许电流且必须大于最小输出电流
     * @param longId 执行器长id
     * @param maxOutputCurrent 设置的最大输出电流
     * @return 如果设置的电流值有效，返回true，否则返回false
     */
    bool setMaxOutputCurrent(uint64_t longId,double maxOutputCurrent);
    /**
     * @brief getMinOutputCurrent 获取执行器最小输出电流，单位为A，该值不会小于执行器最大允许电流的负值
     * @param id 执行器短id
     * @return 执行器最小输出电流，单位为A
     */
    double getMinOutputCurrent(uint8_t id)const;
    /**
     * @brief getMinOutputCurrent 获取执行器最小输出电流，单位为A，该值不会小于执行器最大允许电流的负值
     * @param longId 执行器长id
     * @return 执行器最小输出电流，单位为A
     */
    double getMinOutputCurrent(uint64_t longId)const;
    /**
     * @brief setMinOutputCurrent 设置执行器最最小输出电流，单位为A，该值不会小于执行器最大允许电流的负值且必须小于最大输出电流
     * @param id 执行器短id
     * @param minOutputCurrent 设置的最小输出电流
     * @return 如果设置的电流值有效，返回true，否则返回false
     */
    bool setMinOutputCurrent(uint8_t id,double minOutputCurrent);
    /**
     * @brief setMinOutputCurrent 设置执行器最最小输出电流，单位为A，该值不会小于执行器最大允许电流的负值且必须小于最大输出电流
     * @param longId 执行器长id
     * @param minOutputCurrent 设置的最小输出电流
     * @return 如果设置的电流值有效，返回true，否则返回false
     */
    bool setMinOutputCurrent(uint64_t longId,double minOutputCurrent);
    /**
     * @brief getMaxOutputVelocity 获取执行器最大输出速度，单位为RPM，该值不会大于执行器最大允许速度
     * @param id 执行器短id
     * @return 执行器最大输出速度，单位为RPM
     */
    double getMaxOutputVelocity(uint8_t id)const;
    /**
     * @brief getMaxOutputVelocity 获取执行器最大输出速度，单位为RPM，该值不会大于执行器最大允许速度
     * @param longId 执行器长id
     * @return 执行器最大输出速度，单位为RPM
     */
    double getMaxOutputVelocity(uint64_t longId)const;
    /**
     * @brief setMaxOutputVelocity 设置执行器最大输出速度，单位为RPM，该值不会大于执行器最大允许速度且必须大于最小输出速度
     * @param id 执行器短id
     * @param maxOutputVelocity 设置的最大输出速度
     * @return 如果设置的速度值有效，返回true，否则返回false
     */
    bool setMaxOutputVelocity(uint8_t id,double maxOutputVelocity);
    /**
     * @brief setMaxOutputVelocity 设置执行器最大输出速度，单位为RPM，该值不会大于执行器最大允许速度且必须大于最小输出速度
     * @param id 执行器长id
     * @param maxOutputVelocity 设置的最大输出速度
     * @return 如果设置的速度值有效，返回true，否则返回false
     */
    bool setMaxOutputVelocity(uint64_t id,double maxOutputVelocity);
    /**
     * @brief getMinOutputCurrent 获取执行器最小输出速度，单位为RPM，该值不会小于执行器最大允许速度的负值
     * @param id 执行器短id
     * @return 执行器最小输出速度，单位为RPM
     */
    double getMinOutputVelocity(uint8_t id)const;
    /**
     * @brief getMinOutputCurrent 获取执行器最小输出速度，单位为RPM，该值不会小于执行器最大允许速度的负值
     * @param longId 执行器长id
     * @return 执行器最小输出速度，单位为RPM
     */
    double getMinOutputVelocity(uint64_t longId)const;
    /**
     * @brief setMinOutputCurrent 设置执行器最最小输出速度，单位为RPM，该值不会小于执行器最大允许速度的负值且必须小于最大输出速度
     * @param id 执行器短id
     * @param minOutputCurrent 设置的最小输出速度
     * @return 如果设置的速度值有效，返回true，否则返回false
     */
    bool setMinOutputVelocity(uint8_t id,double minOutputVelocity);
    /**
     * @brief setMinOutputCurrent 设置执行器最最小输出速度，单位为RPM，该值不会小于执行器最大允许速度的负值且必须小于最大输出速度
     * @param longId 执行器长id
     * @param minOutputCurrent 设置的最小输出速度
     * @return 如果设置的速度值有效，返回true，否则返回false
     */
    bool setMinOutputVelocity(uint64_t longId,double minOutputVelocity);
    /**
     * @brief activateActuatorMode 激活单个执行器的指定模式
     * @param id 执行器短id
     * @param nMode 指定的激活模式
     */
    void activateActuatorMode(uint8_t id, const Actuator::ActuatorMode nMode);

    /**
     * @brief activateActuatorMode 激活单个执行器的指定模式
     * @param id 执行器长id
     * @param nMode 指定的激活模式
     */
    void activateActuatorMode(uint64_t id, const Actuator::ActuatorMode nMode);
    /**
     * @brief getActuatorAttributeWithACK 获取执行器属性，等待读取执行器当前属性，如果失败bSuccess的值为false
     * @param id 执行器短id
     * @param attrId 属性Id，参见Actuator::ActuatorAttribute
     * @param bSuccess 查询成功为true，反之为false
     * @return 查询属性的值
     */
    double getActuatorAttributeWithACK(uint8_t id,Actuator::ActuatorAttribute attrId,bool * bSuccess = nullptr);
    /**
     * @brief getActuatorAttributeWithACK 获取执行器属性，等待读取执行器当前属性，如果失败bSuccess的值为false
     * @param longId 执行器长id
     * @param attrId 属性Id，参见Actuator::ActuatorAttribute
     * @param bSuccess 查询成功为true，反之为false
     * @return 查询属性的值
     */
    double getActuatorAttributeWithACK(uint64_t longId,Actuator::ActuatorAttribute attrId,bool * bSuccess = nullptr);
    /**
     * @brief setActuatorAttributeWithACK 设置执行器属性值，等待设置完成
     * @param id 执行器短id
     * @param attrId 属性Id，参见Actuator::ActuatorAttribute
     * @param value 属性值
     * @return 返回设置是否成功
     */
    bool setActuatorAttributeWithACK(uint8_t id, ActuatorAttribute attrId, double value);
    /**
     * @brief setActuatorAttributeWithACK 设置执行器属性值，等待设置完成
     * @param longId 执行器长id
     * @param attrId 属性Id，参见Actuator::ActuatorAttribute
     * @param value 属性值
     * @return 返回设置是否成功
     */
    bool setActuatorAttributeWithACK(uint64_t longId, ActuatorAttribute attrId, double value);
    /**
     * @brief getCVPValue 获取电流速度位置的值(如果同时需要三个值，该接口效率比较高）
     * @param id 执行器短id
     */
    void getCVPValue(uint8_t id);
    /**
     * @brief getCVPValue 获取电流速度位置的值(如果同时需要三个值，该接口效率比较高）
     * @param longId 执行器长id
     */
    void getCVPValue(uint64_t longId);
    //v3.0 add end
    void switchCalibrationVel(uint64_t longId,uint8_t nValue);
    void switchCalibration(uint64_t longId,uint8_t nValue);
    void startCalibration(uint64_t longId);
    void sendCmd(uint64_t longId,uint16_t cmdId,uint32_t value);
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
    void finishRecognizeCallback();
    void onRequestCallback(uint64_t longId, uint8_t nProxyId,double value);
    void errorOccur(uint64_t longId,uint16_t errorId, std::string errorStr);
    void motorAttrChanged(uint64_t longId,uint8_t attrId,double value);
    void startNewChart();
    void chartValueChange(uint8_t channelId,double value);
    void startLog();
    void stopLog();
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
    int m_nLogPid;
};

#endif // MOTORSCONTROLLER_H
