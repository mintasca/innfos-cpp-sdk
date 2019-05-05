/*
*执行器位置归零和限位设置
*/
#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <iostream>
#include <string.h>
using namespace std;

bool bExit = false;
bool bSetLimitation = false;

void processSignal(int sign)
{
    ActuatorController::getInstance()->closeAllActuators();
    //延时保证关机成功
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
}

//执行器0位和限位调整
void setActuatorLimitation()
{
    ActuatorController * pController = ActuatorController::getInstance();
    vector<uint8_t> idArray = pController->getActuatorIdArray();
    for (uint8_t id : idArray) {
        //将执行器当前位置变成0位，并且最小和最大位置分别设置为-10,10,偏移设置为0.5，执行器的运动范围变成（-9.5,9.5）
        pController->setHomingPosition(id,pController->getActuatorAttribute(id,Actuator::ACTUAL_POSITION));
        pController->setMinPosLimit(id,-10);
        pController->setMaxPosLimit(id,10);
        pController->setActuatorAttribute(id,Actuator::POS_OFFSET,0.5);
    }
    bSetLimitation = true;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: homingActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
        return 1;
    }

    //初始化控制器
    if(strcmp(argv[1],"-s")==0)
        ActuatorController::initController(Actuator::Via_Serialport);
    else if(strcmp(argv[1],"-e")==0)
        ActuatorController::initController();
    else
    {
        std::cerr << argv[1] <<"Usage: homingActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
        return 1;
    }



    signal(SIGINT,processSignal);
    //ActuatorController::initController(argc,argv);
    ActuatorController * pController = ActuatorController::getInstance();

    int nLaunchedActuatorCnt = 0;
    double min = 0;
    double max = 0;
    double offset = 0;
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
                            cout << "launch" << nLaunchedActuatorCnt << endl;
                            setActuatorLimitation();
                        }
                    }
                }
            }
            break;
        case Actuator::Launch_Finished:
            //cout << "launch2" << nLaunchedActuatorCnt << endl;
            if(++nLaunchedActuatorCnt == pController->getActuatorIdArray().size())//所有执行器都已启动完成
            {

                setActuatorLimitation();
            }
            break;
        default:
            break;
        }
    });

    int nResponse = 0;
    //关联控制器控制的执行器属性变化信号
    int nAttrConnection = pController->m_sActuatorAttrChanged->s_Connect([&](uint8_t nDeviceId,uint8_t nAttrId,double value){
        if(!bSetLimitation)
            return;
        switch (nAttrId)
        {
        case Actuator::HOMING_POSITION:
            ++ nResponse;
            break;
        case Actuator::POS_LIMITATION_MINIMUM:
            min = value;
            ++ nResponse;
            break;
        case Actuator::POS_LIMITATION_MAXIMUM:
            max = value;
            ++ nResponse;
            break;
        case Actuator::POS_OFFSET:
            offset = value;
            ++ nResponse;
            break;
        default:
            break;
        }
        if(nResponse == 4)
        {
            if(max-min-2*offset > 0)
            {
                cout << "Set Limitation successfully! The actuator's range of movement is :" << min+offset << " " << max-offset << endl;
                pController->setActuatorAttribute(nDeviceId,Actuator::POS_LIMITATION_SWITCH,1);//1为开启位置限制，0为关闭位置限制
                pController->saveAllParams(nDeviceId);
				bSetLimitation = false;
            }
            else
            {
                cout << "Set Limitation failed! " << "Minimum:" << min  <<" "<< "Maximum:" << max  <<" "<< "Offset:" << offset  <<" "<<endl;
            }
        }
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
    }

    //断开信号连接
    pController->m_sOperationFinished->s_Disconnect(nOperationConnection);
    pController->m_sError->s_Disconnect(nErrorConnection);
    pController->m_sActuatorAttrChanged->s_Disconnect(nAttrConnection);

    return 0;
}
