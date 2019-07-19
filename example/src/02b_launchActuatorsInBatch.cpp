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
    //查找已连接的执行器
    std::vector<ActuatorController::UnifiedID> actuators = pController->lookupActuators(ec);

    if(actuators.size() > 0)
    {
        if(pController->launchActuatorInBatch(actuators))
        {
            cout << "All actuators have been launched successfully! " << endl;
        }
        pController->closeAllActuators();
        //insure that all actuators have been closed
        this_thread::sleep_for(std::chrono::milliseconds(200));

    }
    else
    {
        cout << "Connected error code:" << hex << ec << endl;
    }

    return 0;
}
