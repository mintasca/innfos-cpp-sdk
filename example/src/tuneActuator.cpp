/*
*执行器属性调整
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

//执行器属性调整,调整成功，会触发执行器属性变化信号
void tuneActuator()
{
    ActuatorController * pController = ActuatorController::getInstance();
    vector<uint8_t> idArray = pController->getActuatorIdArray();
    for (uint8_t id: idArray) {
        //调整执行器速度环最小电流输出
		pController->setMinOutputCurrent(id, -10);
        //调整执行器速度环最大电流输出
		pController->setMaxOutputCurrent(id, 10);
        //调整执行器位置环最小速度输出
		pController->setMinOutputVelocity(id, -2000);
        //调整执行器位置环最大速度输出，最大值要大于最小值
		pController->setMaxOutputVelocity(id, 2000);
        //调整执行器Mode_Profile_Pos的最大速度（RPM）
        pController->setActuatorAttribute(id,Actuator::PROFILE_POS_MAX_SPEED,1000);
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
                            tuneActuator();
                        }
                    }
                }
            }
            break;
        case Actuator::Launch_Finished:
            if(++nLaunchedActuatorCnt == pController->getActuatorIdArray().size())//所有执行器都已启动完成
            {
                tuneActuator();
            }
            break;
        default:
            break;
        }
    });

    //关联控制器控制的执行器属性变化信号
    int nAttrConnection = pController->m_sActuatorAttrChanged->s_Connect([=](uint8_t nDeviceId,uint8_t nAttrId,double value){
        string description;
        double actualValue = 0;
        string unitStr ;
        switch (nAttrId) {
        case Actuator::VEL_OUTPUT_LIMITATION_MAXIMUM:
            description = "VEL_OUTPUT_LIMITATION_MAXIMUM";
            actualValue = pController->getActuatorAttribute(nDeviceId,Actuator::CURRENT_SCALE)*value;
            unitStr = "A";
            break;
        case Actuator::VEL_OUTPUT_LIMITATION_MINIMUM:
            description = "VEL_OUTPUT_LIMITATION_MINIMUM";
            actualValue = pController->getActuatorAttribute(nDeviceId,Actuator::CURRENT_SCALE)*value;
            unitStr = "A";
            break;
        case Actuator::POS_OUTPUT_LIMITATION_MAXIMUM:
            description = "POS_OUTPUT_LIMITATION_MAXIMUM";
            actualValue = pController->getActuatorAttribute(nDeviceId,Actuator::VELOCITY_SCALE)*value;
            unitStr = "RPM";
            break;
        case Actuator::POS_OUTPUT_LIMITATION_MINIMUM:
            description = "POS_OUTPUT_LIMITATION_MINIMUM";
            actualValue = pController->getActuatorAttribute(nDeviceId,Actuator::VELOCITY_SCALE)*value;
            unitStr = "RPM";
            break;
        case Actuator::PROFILE_POS_MAX_SPEED:
            description = "PROFILE_POS_MAX_SPEED";
            actualValue = value;
            unitStr = "RPM";
            break;
        default:
            break;
        }

        if(description.size() > 0)
        {
            //如果想要参数下次启动依然生效，需要保存参数
            pController->saveAllParams(nDeviceId);
            cout << "Actuator " << (int)nDeviceId <<" Attribute " << description.c_str() << " change to " << actualValue << " " << unitStr <<endl;
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
    pController->m_sActuatorAttrChanged->s_Disconnect(nAttrConnection);
    return 0;
}
