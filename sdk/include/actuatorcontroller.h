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
class ControllerUtil;
#define controllerInst ActuatorController::getInstance()


/**
 * @brief The ActuatorController class 执行器控制器类，所有与执行器相关的操作都在此类中
 */
class ACTUATORCONTROLLERSHARED_EXPORT ActuatorController
{
public:
    ~ActuatorController();
protected:
    ActuatorController();
public:
    /**
     * @brief 执行器唯一ID,由执行器ID(actuatorID)和执行器通信地址(ipAddress)组成
     * @warning 同一通信地址下的执行器ID不能重复，不同通信地址下的执行器ID可重复
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
 * @brief 使能所有执行器
 * @return 全部使能成功返回true，否则返回false
**/
    bool enableAllActuators();
    /**
 * @brief 失能所有执行器
 * @return 全部失能成功返回true，否则返回false
**/
    bool disableAllActuators();
/**
 * @brief 使能指定执行器
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
 * @warning
**/
    bool enableActuator(uint8_t id,const string & ipAddress="");
    /**
     * @brief 使能指定执行器
     * @param unifiedIDArray 执行器UnifiedID数组
     * @return 执行器使能成功返回true,否则返回false
    **/
    bool enableActuatorInBatch(const vector<UnifiedID>& unifiedIDArray);
    /**
 * @brief 失能指定执行器
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
 * @return 执行器失能成功返回true,否则返回false
**/
    bool disableActuator(uint8_t id,const string & ipAddress="");

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
     * @param UnifiedIDArray 执行器UnifiedID数组
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
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次位置读取,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前位置，单位是转数
**/
    double getPosition(uint8_t id,bool bRefresh,const string & ipAddress="")const;

    /**
     * @brief 设置位置环的比例
     * @param id 执行器id
     * @param Kp 位置环的比例
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionKp(uint8_t id,double Kp,const string & ipAddress="");
    /**
     * @brief 获取位置环的比例
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次位置环的比例,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的比例
     */
    double getPositionKp(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置位置环的积分
     * @param id 执行器id
     * @param Ki 位置环的积分
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionKi(uint8_t id,double Ki,const string & ipAddress="");
    /**
     * @brief 获取位置环的积分
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次位置环的积分,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的积分
     */
    double getPositionKi(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置位置环的微分
     * @param id 执行器id
     * @param Kd 位置环的微分
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionKd(uint8_t id,double Kd,const string & ipAddress="");
    /**
     * @brief 获取位置环的微分
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次位置环的微分,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的微分
     */
    double getPositionKd(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置位置环的最大输出限幅
     * @param id 执行器id
     * @param max 位置环的最大输出限幅,有效值范围为(0,1]
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionUmax(uint8_t id,double max,const string & ipAddress="");
    /**
     * @brief 获取位置环的最大输出限幅
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次最大输出限幅,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的最大输出限幅
     */
    double getPositionUmax(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置位置环的最小输出限幅
     * @param id 执行器id
     * @param min 位置环的最小输出限幅,有效值范围为[-1,0)
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionUmin(uint8_t id,double min,const string & ipAddress="");
    /**
     * @brief 获取位置环的最小输出限幅
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次最小输出限幅,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的最小输出限幅
     */
    double getPositionUmin(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置位置环的限位偏移
     * @param id 执行器id
     * @param offset 位置环的限位偏移
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionOffset(uint8_t id, double offset, const string & ipAddress="");
    /**
     * @brief 获取位置环的限位偏移
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次限位偏移,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的限位偏移
     */
    double getPositionOffset(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置位置环的最大限位
     * @param id 执行器id
     * @param maxPos 位置环的最大限位
     * @param ipAddress 目标ip地址字符串
     */
    void setMaximumPosition(uint8_t id,double maxPos,const string & ipAddress="");
    /**
     * @brief 获取位置环的最大限位
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次最大限位,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的最大限位
     */
    double getMaximumPosition(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置位置环的最小限位
     * @param id 执行器id
     * @param minPos 位置环的最小限位
     * @param ipAddress 目标ip地址字符串
     */
    void setMinimumPosition(uint8_t id,double minPos,const string & ipAddress="");
    /**
     * @brief 获取位置环的最小限位
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次最小限位,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环的最小限位
     */
    double getMinimumPosition(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 使能/失能执行器限位功能，失能后速度模式和电流模式将不受限位影响
     * @param id 执行器id
     * @param enable 使能/失能
     * @param ipAddress ipAddress 目标ip地址字符串
     */
    void enablePositionLimit(uint8_t id, bool enable, const string & ipAddress="");
    /**
     * @brief 读取执行器限位功能使能/失能
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次限位功能使能/失能,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return
     */
    bool isPositionLimitEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置执行器的零位
     * @param id 执行器id
     * @param homingPos 执行器的零位
     * @param ipAddress 目标ip地址字符串
     */
    void setHomingPosition(uint8_t id,double homingPos,const string & ipAddress="");
    /**
     * @brief 使能/失能位置环滤波功能，该功能为一阶低通滤波
     * @param id 执行器id
     * @param enable 使能/失能
     * @param ipAddress ipAddress 目标ip地址字符串
     */
    void enablePositionFilter(uint8_t id,bool enable,const string & ipAddress="");
    /**
     * @brief 读取执位置环滤波功能使能/失能
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次位置环滤波功能使能/失能,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return
     */
    bool isPositionFilterEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 获取位置环低通滤波频率
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次低通滤波频率,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 位置环低通滤波频率
     */
    double getPositionCutoffFrequency(uint8_t id, bool bRefresh, const string &ipAddress="") const;
    /**
     * @brief 设置位置环低通滤波频率
     * @param id 执行器id
     * @param frequency 位置环低通滤波频率
     * @param ipAddress 目标ip地址字符串
     */
    void setPositionCutoffFrequency(uint8_t id, double frequency, const string &ipAddress="");
    /**
 * @brief 清除homing信息，包括左右极限和0位
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
**/
    void clearHomingInfo(uint8_t id,const string & ipAddress="");
    //profile position
    /**
     * @brief 设置Profile position模式的加速度
     * @param id 执行器id
     * @param acceleration Profile position模式的加速度
     * @param ipAddress 目标ip地址字符串
     */
    void setProfilePositionAcceleration(uint8_t id, double acceleration, const string & ipAddress="");
    /**
     * @brief 获取Profile position模式的加速度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次Profile position模式的加速度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return Profile position模式的加速度
     */
    double getProfilePositionAcceleration(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置Profile position模式的减速度
     * @param id 执行器id
     * @param deceleration Profile position模式的减速度
     * @param ipAddress 目标ip地址字符串
     */
    void setProfilePositionDeceleration(uint8_t id, double deceleration, const string & ipAddress="");
    /**
     * @brief 获取Profile position模式的减速度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次Profile position模式的减速度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return Profile position模式的减速度
     */
    double getProfilePositionDeceleration(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置Profile position模式的最大速度
     * @param id 执行器id
     * @param maxVelocity Profile position模式的最大速度
     * @param ipAddress 目标ip地址字符串
     */
    void setProfilePositionMaxVelocity(uint8_t id, double maxVelocity, const string & ipAddress="");
    /**
     * @brief 获取Profile position模式的最大速度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次Profile position模式的最大速度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return Profile position模式的最大速度
     */
    double getProfilePositionMaxVelocity(uint8_t id,bool bRefresh,const string & ipAddress="")const;
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
 * @param bRefresh 是否需要刷新,如果为true，会自动请求一次速度读取,并等待返回，如果为false,则会立即返回最近一次请求速度返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前速度，单位是转/每分钟
**/
    double getVelocity(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 获取速度环比例
 * @param id 执行器id
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次速度环比例读取,并等待返回，如果为false,则会立即返回最近一次请求速度环比例返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前速度环比例
**/
    double getVelocityKp(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 设置速度环比例
 * @param id 执行器id
 * @param Kp 速度环比例
 * @param ipAddress 目标ip地址字符串
**/
    void setVelocityKp(uint8_t id,double Kp,const string & ipAddress="");

    /**
 * @brief 获取速度环积分
 * @param id 执行器id
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次速度环积分读取,并等待返回，如果为false,则会立即返回最近一次请求速度环积分返回的结果
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
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次速度环最大输出限幅读取,并等待返回，如果为false,则会立即返回最近一次请求速度环最大输出限幅返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 最大输出限幅
**/
    double getVelocityUmax(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置速度环最大输出限幅
     * @param id 执行器id
     * @param max 最大输出限幅,有效值范围为（0,1]
     * @param ipAddress 目标ip地址字符串
     * @return 设置成功或失败
     */
    void setVelocityUmax(uint8_t id, double max, const string &ipAddress="");

    /**
 * @brief 获取速度环最小输出限幅
 * @param id 执行器id
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次速度环最小输出限幅读取,并等待返回，如果为false,则会立即返回最近一次请求速度环最小输出限幅返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 最小输出限幅
**/
    double getVelocityUmin(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置速度环最小输出限幅
     * @param id 执行器id
     * @param min 最小输出限幅,有效值范围为[-1,0)
     * @param ipAddress 目标ip地址字符串
     * @return 设置成功或失败
     */
    void setVelocityUmin(uint8_t id, double min, const string &ipAddress="");
    /**
     * @brief 获取执行器速度量程，单位RPM
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次执行器速度量程,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 执行器速度量程
     */
    double getVelocityRange(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 使能/失能速度环滤波功能，该功能为一阶低通滤波
     * @param id 执行器id
     * @param enable 使能/失能
     * @param ipAddress ipAddress 目标ip地址字符串
     */
    void enableVelocityFilter(uint8_t id,bool enable,const string & ipAddress="");
    /**
     * @brief 读取执速度环滤波功能使能/失能
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次速度环滤波功能使能/失能,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 速度环滤波功能使能/失能
     */
    bool isVelocityFilterEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 获取速度环低通滤波频率
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次低通滤波频率,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 速度环低通滤波频率
     */
    double getVelocityCutoffFrequency(uint8_t id, bool bRefresh, const string &ipAddress="") const;
    /**
     * @brief 设置速度环低通滤波频率
     * @param id 执行器id
     * @param frequency 速度环低通滤波频率
     * @param ipAddress 目标ip地址字符串
     */
    void setVelocityCutoffFrequency(uint8_t id, double frequency, const string &ipAddress="");
    /**
     * @brief 设置执行器速度限制
     * @param id 执行器id
     * @param limit 执行器速度限制，单位是RPM,该值不会超过速度量程
     * @param ipAddress 目标ip地址字符串
     */
    void setVelocityLimit(uint8_t id,double limit,const string & ipAddress="");
    /**
     * @brief 获取执行器速度限制
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次执行器速度限制,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 执行器速度限制
     */
    double getVelocityLimit(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    //profile velocity
    /**
     * @brief 设置Profile velocity模式的加速度
     * @param id 执行器id
     * @param acceleration Profile velocity模式的加速度
     * @param ipAddress 目标ip地址字符串
     */
    void setProfileVelocityAcceleration(uint8_t id,double acceleration,const string & ipAddress="");
    /**
     * @brief 获取Profile velocity模式的加速度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次Profile velocity模式的加速度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return Profile velocity模式的加速度
     */
    double getProfileVelocityAcceleration(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置Profile velocity模式的减速度
     * @param id 执行器id
     * @param deceleration Profile velocity模式的减速度
     * @param ipAddress 目标ip地址字符串
     */
    void setProfileVelocityDeceleration(uint8_t id,double deceleration,const string & ipAddress="");
    /**
     * @brief 获取Profile velocity模式的减速度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次Profile velocity模式的减速度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return Profile velocity模式的减速度
     */
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
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次电流读取,并等待返回，如果为false,则会立即返回最近一次请求电流返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前电流，单位是A
**/
    double getCurrent(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 获取电流环比例
 * @param id 执行器id
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次电流环比例读取,并等待返回，如果为false,则会立即返回最近一次请求电流环比例返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前电流环比例
**/
    double getCurrentKp(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 获取电流环积分
 * @param id 执行器id
 * @param bRefresh 是否需要刷新，如果为true，会自动请求一次电流环积分读取,并等待返回，如果为false,则会立即返回最近一次请求电流环积分返回的结果
 * @param ipAddress 目标ip地址字符串
 * @return 当前电流环积分
**/
    double getCurrentKi(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 获取执行器电流量程，单位A
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次执行器电流量程,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 执行器电流量程
     */
    double getCurrentRange(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 使能/失能电流环滤波功能，该功能为一阶低通滤波
     * @param id 执行器id
     * @param enable 使能/失能
     * @param ipAddress ipAddress 目标ip地址字符串
     */
    void enableCurrentFilter(uint8_t id,bool enable,const string & ipAddress="");
    /**
     * @brief 读取执电流环滤波功能使能/失能
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次电流环滤波功能使能/失能,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 电流环滤波功能使能/失能
     */
    bool isCurrentFilterEnable(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 获取电流环低通滤波频率
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次低通滤波频率,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 电流环低通滤波频率
     */
    double getCurrentCutoffFrequency(uint8_t id, bool bRefresh, const string &ipAddress="") const;
    /**
     * @brief 设置电流环低通滤波频率
     * @param id 执行器id
     * @param frequency 电流环低通滤波频率
     * @param ipAddress 目标ip地址字符串
     */
    void setCurrentCutoffFrequency(uint8_t id, double frequency, const string &ipAddress="");
    /**
     * @brief 设置执行器电流限制
     * @param id 执行器id
     * @param limit 执行器电流限制,单位是A，该值不会超过电流量程
     * @param ipAddress 目标ip地址字符串
     */
    void setCurrentLimit(uint8_t id,double limit,const string & ipAddress="");
    /**
     * @brief 获取执行器电流限制
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次执行器电流限制,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 执行器电流限制
     */
    double getCurrentLimit(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
 * @brief 执行器保存当前所有参数,如果修改参数以后没有保存，失能后将丢弃参数修改
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
 * @return 保存成功返回true,否则返回false
**/
    bool saveAllParams(uint8_t id,const string & ipAddress="");

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
 * @param ipAddress 目标ip地址字符串
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
**/
    void openChartChannel(uint8_t id,uint8_t  nChannelId,const string & ipAddress="");

    /**
 * @brief 关闭图表指定通道
 * @param id 执行器id
 * @param nChannelId 通道id（Actuator::channel_1到Actuator::channel_4）
 * @param ipAddress 目标ip地址字符串
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
    /**
     * @brief 获取执行器电压
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次执行器电压,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 执行器电压
     */
    double getVoltage(uint8_t id,bool bRefresh,const string & ipAddress="")const;

    /**
     * @brief 获取执行器堵转能量
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次执行器堵转能量,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 执行器堵转能量，单位J
     */
    double getLockEnergy(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置执行器堵转能量
     * @param id 执行器id
     * @param energy 执行器堵转能量，单位J
     * @param ipAddress 目标ip地址字符串
     */
    void setLockEnergy(uint8_t id,double energy,const string & ipAddress="");
    /**
     * @brief 获取电机温度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次电机温度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 电机温度,单位℃
     */
    double getMotorTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 获取逆变器温度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次逆变器温度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 逆变器温度,单位℃
     */
    double getInverterTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 获取电机保护温度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次电机保护温度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 电机保护温度,单位℃
     */
    double getMotorProtectedTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置电机保护温度
     * @param id 执行器id
     * @param temp 电机保护温度
     * @param ipAddress 目标ip地址字符串
     */
    void setMotorProtectedTemperature(uint8_t id,double temp,const string & ipAddress="");
    /**
     * @brief 获取电机恢复温度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次电机恢复温度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 电机恢复温度,单位℃
     */
    double getMotorRecoveryTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置电机恢复温度
     * @param id 执行器id
     * @param temp 电机恢复温度
     * @param ipAddress 目标ip地址字符串
     */
    void setMotorRecoveryTemperature(uint8_t id,double temp,const string & ipAddress="");
    /**
      * @brief 获取逆变器保护温度
      * @param id 执行器id
      * @param bRefresh 是否需要刷新，如果为true，会自动请求一次逆变器保护温度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
      * @param ipAddress 目标ip地址字符串
      * @return 逆变器保护温度,单位℃
      */
    double getInverterProtectedTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置逆变器保护温度
     * @param id 执行器id
     * @param temp 逆变器保护温度
     * @param ipAddress 目标ip地址字符串
     */
    void setInverterProtectedTemperature(uint8_t id,double temp,const string & ipAddress="");
    /**
     * @brief 获取逆变器恢复温度
     * @param id 执行器id
     * @param bRefresh 是否需要刷新，如果为true，会自动请求一次逆变器恢复温度,并等待返回，如果为false,则会立即返回最近一次请求位置返回的结果
     * @param ipAddress 目标ip地址字符串
     * @return 逆变器恢复温度,单位℃
     */
    double getInverterRecoveryTemperature(uint8_t id,bool bRefresh,const string & ipAddress="")const;
    /**
     * @brief 设置逆变器恢复温度
     * @param id 执行器id
     * @param temp 逆变器恢复温度
     * @param ipAddress 目标ip地址字符串
     */
    void setInverterRecoveryTemperature(uint8_t id,double temp,const string & ipAddress="");
    /**
     * @brief 执行器是否在线
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     * @return 在线状态
     */
    bool isOnline(uint8_t id,const string & ipAddress="")const;
    /**
     * @brief 执行器是否已经使能
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     * @return 是否使能
     */
    bool isEnable(uint8_t id,const string & ipAddress="")const;
    /**
     * @brief 使能执行器心跳功能，使能后自动刷新执行器在线状态和错误（默认状态为使能）
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     */
    void enableHeartbeat(uint8_t id,const string & ipAddress="")const;
    /**
     * @brief 失能执行器心跳功能，失能后关闭自动刷新执行器在线状态和错误
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     */
    void disableHeartbeat(uint8_t id,const string & ipAddress="")const;
    /**
     * @brief 设置执行器ID
     * @param currentID 当前执行器id
     * @param newID 执行器新ID
     * @param ipAddress 目标ip地址字符串
     * @return 是否修改成功
     */
    bool setActuatorID(uint8_t currentID, uint8_t newID, const string & ipAddress="");
    /**
     * @brief 获取执行器序列号
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     * @return 执行器序列号
     */
    uint32_t getActuatorSerialNumber(uint8_t id,const string & ipAddress="")const;
    /**
     * @brief 获取执行器当前模式
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     * @return 当前模式
     */
    Actuator::ActuatorMode getActuatorMode(uint8_t id,const string & ipAddress="")const;
    /**
     * @brief 获取执行器错误代码
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     * @return 错误代码
     */
    uint32_t getErrorCode(uint8_t id,const string & ipAddress="")const;
    /**
 * @brief 重新获取属性,将请求刷新属性
 * @param id 执行器id
 * @param attrId 执行器属性Id
 * @param ipAddress 目标ip地址字符串
**/
    void regainAttribute(uint8_t id,uint8_t attrId,const string & ipAddress="");

    /**
 * @brief 获取执行器错误历史记录
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
**/
    vector<uint16_t> getErrorHistory(uint8_t id,const string & ipAddress="");

    /**
 * @brief 执行器掉线重连
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
**/
    void reconnect(uint8_t id,const string & ipAddress="");

    /**
 * @brief 执行器错误清除
 * @param id 执行器id
 * @param ipAddress 目标ip地址字符串
**/
    void clearError(uint8_t id,const string & ipAddress="");

    /**
     * @brief versionString 获取sdk版本号字符串
     * @return sdk版本号字符串
     */
    string versionString()const;


    /**
     * @brief requestCVPValue 获取电流速度位置的值(如果同时需要三个值，该接口效率比较高）
     * @param id 执行器id
     * @param ipAddress 目标ip地址字符串
     */
    void requestCVPValue(uint8_t id,const string & ipAddress="");


    using doubleFuncPointer = void(*)(UnifiedID, uint8_t ,double);
    using stringFuncPointer = void(*)(UnifiedID, uint16_t ,string);
    using doubleFunction = function<void(UnifiedID, uint8_t ,double)>;
    using stringFunction = function<void(UnifiedID, uint16_t ,string)>;

    /**
     * @brief addParaRequestCallback 增加参数请求回调函数，当参数请求返回结果后会触发回调
     * @param callback 回调函数指针
     */
    void addParaRequestCallback(doubleFuncPointer callback);
    /**
     * @brief addParaRequestCallback 增加参数请求回调函数，当参数请求返回结果后会触发回调
     * @param callback 回调函数function
     */
    void addParaRequestCallback(doubleFunction callback);
    /**
     * @brief addErrorCallback 增加错误回调函数，当执行器发生错误后会触发回调
     * @param callback 回调函数指针
     * @warning 当失能执行器heartbeat后，即调用disableHeartbeat后将不会上报错误
     */
    void addErrorCallback(stringFuncPointer callback);
    /**
     * @brief addErrorCallback 增加错误回调函数，当执行器发生错误后会触发回调
     * @param callback 回调函数function
     * @warning 当失能执行器heartbeat后，即调用disableHeartbeat后将不会上报错误
     */
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
    void setActuatorAttribute(uint64_t longId,Actuator::ActuatorAttribute attrId,double value);
    void setActuatorAttribute(uint8_t id,Actuator::ActuatorAttribute attrId,double value,const string & ipAddress="");
    double getActuatorAttribute(uint64_t longId,Actuator::ActuatorAttribute attrId)const;
    double getActuatorAttribute(uint8_t id,Actuator::ActuatorAttribute attrId,const string & ipAddress="")const;
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
    void regainAttribute(uint64_t longId,uint8_t attrId);
    void switchChartAllChannel(uint64_t longId,bool bOn);
    void closeChartChannel(uint64_t longId, uint8_t nChannelId);
    void openChartChannel(uint64_t longId,uint8_t  nChannelId);
    void setMinPosLimit(uint64_t longId,double posValue);
    void setMinPosLimit(uint64_t longId);
    void setHomingOperationMode(uint8_t id,uint8_t nMode);
    void setHomingOperationMode(uint64_t longId,uint8_t nMode);
    void clearHomingInfo(uint64_t longId);
    bool saveAllParams(uint64_t id);

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
    int m_nLogPid;
    ControllerUtil * util;
};

#endif // MOTORSCONTROLLER_H
