/*
*同步响应
*/
#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string>
#include <functional>
using namespace std;

bool bExit = false;
void processSignal(int sign)
{
    ActuatorController::getInstance()->closeAllActuators();
    //延时保证关机成功
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
}

//同步响应
void feedback_sync()
{
    ActuatorController * pController = ActuatorController::getInstance();
    vector<uint8_t> idArray = pController->getActuatorIdArray();
    for (uint8_t id: idArray) {
        bool bSuccess = false;
        //读取当前电流，并等待返回，如果读取失败 bSuccess的值为false
        double current = pController->getActuatorAttributeWithACK(id,Actuator::ACTUAL_CURRENT,&bSuccess);
        if(bSuccess)
            cout << "current is " << current << endl;
        //设置速度环最大电流输出，并等待返回，如果设置失败，bSuccess的值为false。（设置速度、位置、电流不能使用此接口）
        bSuccess = pController->setActuatorAttributeWithACK(id,Actuator::VEL_OUTPUT_LIMITATION_MAXIMUM,10);
        if(bSuccess)
            cout << "Set VEL_OUTPUT_LIMITATION_MAXIMUM to 10A" << endl;
    }
}

void requestFeedback(uint8_t id, uint8_t attr,double value)
{
    switch (attr) {
    case Actuator::ACTUAL_POSITION:
        std::cout << "The position of actuator " << uint32_t(id) << " is " << value <<std::endl;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT,processSignal);
    Actuator::ErrorsDefine ec;
    ActuatorController * pController = ActuatorController::lookupActuators(ec);
    vector<uint8_t> idArray = pController->getActuatorIdArray();

    if(idArray.size() > 0)
    {
        for(auto id : idArray)
            pController->launchActuatorWithAcknowledgement(id);

        std::function<void(uint8_t, uint8_t,double)> feedback = std::bind(requestFeedback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pController->addFeedbackHandler<uint8_t,uint8_t,double>(ActuatorController::Request_feedback,feedback);
        for(auto id : idArray)
        {
            pController->regainAttrbute(id,Actuator::ACTUAL_POSITION);
        }

    }
    else
    {
        std::cout << "No available actuator, error code  " << std::hex << ec <<std::endl;
    }

    //执行控制器事件循环
    while (!bExit)
    {
        ActuatorController::processEvents();
    }

    return 0;
}
