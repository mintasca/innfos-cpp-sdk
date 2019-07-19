/*
*自动识别已连接执行器
*/

#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string.h>
#include <chrono>
using namespace std;


int main(int argc, char *argv[])
{
    //初始化控制器
    ActuatorController * pController = ActuatorController::initController();
    //ec 定义一个错误的类型，ec==0x00 代表无错误，ec会以引用的方式传递给pController->lookupActuators(ec)， 当错误发生时，ec的值会被sdk修改为相应的错误代码
    Actuator::ErrorsDefine ec;
    //查找已连接的执行器， 返回查找到的所有执行器的UnifiedID，UnifiedID是由执行器ID(actuatorID)和ECB(ECU)的IP(ipAddress)组成的结构体
    std::vector<ActuatorController::UnifiedID> actuators = pController->lookupActuators(ec);

    if(actuators.size() > 0)
    {
        ActuatorController::UnifiedID actuator = actuators.at(0);
        //launch actuator
        pController->launchActuator(actuator.actuatorID,actuator.ipAddress);
        //activate current mode
        pController->activateActuatorMode(actuator.actuatorID,Actuator::Mode_Cur);

        cout << "set current to 0.6A" << endl;
        pController->setCurrent(actuator.actuatorID,0.6);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "set current to -0.6A" << endl;
        pController->setCurrent(actuator.actuatorID,-0.6);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        pController->closeAllActuators();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    else
    {
        cout << "Connected error code:" << hex << ec << endl;
    }

    return 0;
}
