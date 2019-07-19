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

        pController->closeActuator(actuators.at(0));
        //insure that the actuator has been closed
        this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    else
    {
        //ec=0x803 与ECB(ECU)通信失败
        //ec=0x802 ECB(ECU)与执行器通信失败
        cout << "Connected error code:" << hex << ec << endl;
    }

    return 0;
}
