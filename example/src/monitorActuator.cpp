/*
*监测执行器属性（例如：电流、速度、位置）
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
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
}

void autoRefresh()
{
    ActuatorController * pController = ActuatorController::getInstance();
    vector<uint8_t> idArray = pController->getActuatorIdArray();
    for (uint8_t id: idArray) {
        //启动执行器的自动刷新属性功能
        pController->setAutoRefreshInterval(id,1000);
        pController->switchAutoRefresh(id,true);
        //也可以指定刷新某个属性
        pController->regainAttrbute(id,Actuator::ACTUAL_CURRENT);
    }
}


int main(int argc, char *argv[])
{

    signal(SIGINT,processSignal);
    if(argc != 2)
    {
        std::cerr << "Usage: monitorActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
        return 1;
    }

    //初始化控制器
    if(strcmp(argv[1],"-s")==0)
        ActuatorController::initController(Actuator::Via_Serialport);
    else if(strcmp(argv[1],"-e")==0)
        ActuatorController::initController();
    else
    {
        std::cerr << argv[1] <<"Usage: monitorActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
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
				cout << "Number of connected actuators:" << idArray.size() << endl;
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
                            autoRefresh();
                        }
                    }
                }
            }
            break;
        case Actuator::Launch_Finished:
            if(++nLaunchedActuatorCnt == pController->getActuatorIdArray().size())//所有执行器都已启动完成
            {
                autoRefresh();
            }
            break;
        default:
            break;
        }
    });

    //关联控制器控制的执行器属性变化信号
    int nAttrConnection =pController->m_sActuatorAttrChanged->s_Connect([=](uint8_t nDeviceId,uint8_t nAttrId,double value){
        cout << "Actuator ID: " << (int)nDeviceId << endl;
        cout << "atribute ID: " << (int)nAttrId << endl;
        cout << "atribute value: " << value << endl;
        cout << "----------------------------"<<endl;
    });

    //关联错误信号
    int nErrorConnection = pController->m_sError->s_Connect([=](uint8_t nDeviceId,uint16_t nErrorType,string errorInfo){
        if(nDeviceId==0)
        {
            cout << "Error: " << (int)nErrorType << " " << errorInfo.c_str() << endl;
        }
        else
        {
            cout << "Actuator " << (int)nDeviceId << " " <<"error " << (int)nErrorType << " " << errorInfo.c_str() << endl;
        }
    });

    //自动识别已连接执行器
    pController->autoRecoginze();

    //执行控制器事件循环
    while (!bExit)
    {
        ActuatorController::processEvents();
		this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //pController->closeAllActuators();

    //断开信号连接
    pController->m_sOperationFinished->s_Disconnect(nOperationConnection);
    pController->m_sError->s_Disconnect(nErrorConnection);
    pController->m_sActuatorAttrChanged->s_Disconnect(nAttrConnection);

    return 0;
}

