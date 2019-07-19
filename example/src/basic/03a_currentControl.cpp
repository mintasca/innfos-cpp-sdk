/*
*执行器电流控制
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
    //查找已连接的执行器
    Actuator::ErrorsDefine ec;
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
