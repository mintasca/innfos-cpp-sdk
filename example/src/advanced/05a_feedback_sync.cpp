/*
*同步获取
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
            cout << "Launch actuator " << (int)actuators.at(0) << " successfully!" << endl;
        }
        //同步方式获取执行器当前速度和电流，以同步方式获取执行器的参数，有一定的通信阻塞时间，适用于低频率读取控制执行器，如果想高频读取控制执行器，请使用异步回调方式
        //请求执行器当前位置，第二个参数为true，将向执行器发送读取位置请求并等待返回
        double pos = pController->getPosition(actuatorID,true);
        //请求执行器当前电流，第二个参数为true，将向执行器发送读取位置请求并等待返回
        double cur = pController->getCurrent(actuatorID,true);
        cout << "current postion:" << pos << "R current current:" << cur << "A" <<endl;


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
