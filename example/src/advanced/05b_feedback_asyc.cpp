/*
*异步回调
*/

#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string.h>
#include <chrono>
using namespace std;

bool bExit = false;
void processSignal(int sign)
{
    ActuatorController::getInstance()->closeAllActuators();
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
}

//
void paramFeedback(ActuatorController::UnifiedID uID,uint8_t paramType,double paramValue)
{
    switch (paramType) {
    case Actuator::ACTUAL_CURRENT:
        cout << "Actuator " << (int)uID.actuatorID << " current is " << paramValue << "A"<<endl;
        break;
    case Actuator::ACTUAL_POSITION:
        cout << "Actuator " << (int)uID.actuatorID << " position is " << paramValue << "R"<<endl;
        break;
    case Actuator::ACTUAL_VELOCITY:
        cout << "Actuator " << (int)uID.actuatorID << " velocity is " << paramValue << "RPM"<<endl;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    //初始化控制器，
    ActuatorController * pController = ActuatorController::initController();
    //ec 定义一个错误的类型，ec==0x00 代表无错误，ec会以引用的方式传递给pController->lookupActuators(ec)， 当错误发生时，ec的值会被sdk修改为相应的错误代码
    Actuator::ErrorsDefine ec;
    //查找已连接的执行器
    pController->lookupActuators(ec);
    //获取所有执行器ID数组
    vector<uint8_t> actuators = pController->getActuatorIdArray();

    if(actuators.size() > 0)
    {
        if(pController->launchActuator(actuators.at(0)))
        {
            cout << "Launch actuator " << (int)actuators.at(0) << " successfully!" << endl;
        }

        pController->addParamChangeCallback(paramFeedback);

        while (!bExit) {
            //事件轮询，轮询回调事件，事件触发调用相应回调函数
            ActuatorController::processEvents();
            //异步请求执行器电流，速度，位置,当请求返回后，会通过轮询回调事件触发回调函数
            pController->getCVPValue(actuators.at(0));

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    else
    {
        //ec=0x803 与ECB(ECU)通信失败
        //ec=0x802 ECB(ECU)与执行器通信失败
        cout << "Connected error code:" << hex << ec << endl;
    }

    return 0;
}
