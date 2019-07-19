/*
*自动识别已连接执行器
*/

#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string.h>
using namespace std;

bool bExit = false;
void processSignal(int sign)
{
    ActuatorController::getInstance()->closeAllActuators();
    //延时保证关机成功
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
}

void onParamChanged(uint8_t id ,uint8_t type,double value)
{
    std::cout << "hhhhh" << (int)type << std::endl;
}

int main(int argc, char *argv[])
{
//    Actuator::ErrorsDefine ec;
//    ActuatorController * pController = ActuatorController::lookupActuators(ec);
//    pController->addFeedbackHandler(ActuatorController::Params_Changed,onParamChanged);
//    //pController->addParamChangeCallback(onParamChanged);
//    pController->launchAllActuators();

//    //执行控制器事件循环
//    while (!bExit)
//    {
//        ActuatorController::processEvents();
//    }
    //断开信号连接
//    pController->m_sOperationFinished->s_Disconnect(nOperationConnection);
//    pController->m_sError->s_Disconnect(nErrorConnection);

    return 0;
}
