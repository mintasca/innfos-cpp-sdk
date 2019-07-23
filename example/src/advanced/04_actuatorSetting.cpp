/*
*执行器参数设置
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
        uint8_t actuatorID = actuators.at(0);
        if(pController->launchActuator(actuatorID))
        {
            cout << "Launch actuator " << (int)actuatorID << " successfully!" << endl;
        }
        //activate profile position mode
        pController->activateActuatorMode(actuatorID,Actuator::Mode_Profile_Pos);

        //change acceleration to 1200 RPM/s
        pController->setProfilePosAcceleration(actuatorID,1200);
        //change deceleration to -1200 RPM/s
        pController->setProfilePosDeceleration(actuatorID,-1200);
        //change max velocity to 3000 RPM/s
        pController->setProfilePosMaxVelocity(actuatorID,3000);
        cout << "change position in high speed " << endl;
        pController->setPosition(actuatorID,10);
        this_thread::sleep_for(std::chrono::seconds(4));

        //change acceleration to 300 RPM/s
        pController->setProfilePosAcceleration(actuatorID,300);
        //change deceleration to -300 RPM/s
        pController->setProfilePosDeceleration(actuatorID,-300);
        //change max velocity to 500 RPM/s
        pController->setProfilePosMaxVelocity(actuatorID,500);
        cout << "change position in low speed " << endl;
        pController->setPosition(actuatorID,-10);
        this_thread::sleep_for(std::chrono::seconds(5));

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
