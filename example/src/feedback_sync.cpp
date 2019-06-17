/*
*同步响应
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

int main(int argc, char *argv[])
{
    signal(SIGINT,processSignal);
    if(argc != 2)
    {
        std::cerr << "Usage: tuneActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
        return 1;
    }

    //初始化控制器
    if(strcmp(argv[1],"-s")==0)
        ActuatorController::initController(Actuator::Via_Serialport);
    else if(strcmp(argv[1],"-e")==0)
        ActuatorController::initController();
    else
    {
        std::cerr << argv[1] <<"Usage: tuneActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
        return 1;
    }

    ActuatorController * pController = ActuatorController::getInstance();

    int nLaunchedActuatorCnt = 0;
    //关联控制器的操作信号
    int nOperationConnection = pController->m_sOperationFinished->s_Connect([&](uint8_t nDeviceId,uint8_t operationType){
        switch (operationType) {
        case Actuator::Recognize_Finished://自动识别完成
            if(pController->hasAvailableActuator())
            {
                vector<uint8_t> idArray = pController->getActuatorIdArray();
                for (uint8_t id: idArray) {
                    if(pController->getActuatorAttribute(id,Actuator::ACTUATOR_SWITCH)==Actuator::ACTUATOR_SWITCH_OFF)
                    {//如果执行器处于关机状态，启动执行器
                        pController->launchActuator(id);
                    }
                    else
                    {
                        ++ nLaunchedActuatorCnt;
                        if(nLaunchedActuatorCnt == pController->getActuatorIdArray().size())//所有执行器都已启动完成
                        {
                            feedback_sync();
                        }
                    }
                }
            }
            break;
        case Actuator::Launch_Finished:
            if(++nLaunchedActuatorCnt == pController->getActuatorIdArray().size())//所有执行器都已启动完成
            {
                feedback_sync();
            }
            break;
        default:
            break;
        }
    });


    //关联错误信号
    int nErrorConnection = pController->m_sError->s_Connect([=](uint8_t nDeviceId,uint16_t nErrorType,string errorInfo){
        if(nDeviceId==0)
        {
            cout << "Error: " << (int)nErrorType << " " << errorInfo << endl;
        }
        else
        {
            cout << "Actuator " << (int)nDeviceId << " " <<"error " << (int)nErrorType << " " << errorInfo << endl;
        }
    });


    //自动识别已连接执行器
    pController->autoRecoginze();
    //执行控制器事件循环
    while (!bExit)
    {
        ActuatorController::processEvents();
    }

    //断开信号连接
    pController->m_sOperationFinished->s_Disconnect(nOperationConnection);
    pController->m_sError->s_Disconnect(nErrorConnection);
    return 0;
}
