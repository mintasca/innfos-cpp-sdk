/*
*简单操作执行器
*/
#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <atomic>
#include <string>
using namespace std;

atomic_bool bExit{false};

atomic_uchar directive{0xff};
atomic_int value{0};

void keyboardInput()
{
    char d;
    double v;
    while (!bExit)
    {
        if(true)
        {
            cin >> d;
            cin >> v;
            if(!cin.fail())
            {
                value = v*1000;
                directive = d;
            }
        }
    }
}

void cmdOperation(char directive,double value)
{
    vector<uint8_t> idArray = controllerInst->getActuatorIdArray();
    switch (directive)
    {
    case 'a'://激活执行器指定模式，指令格式：a 模式id（Actuator::ActuatorMode）
        controllerInst->activeActuatorMode(idArray, Actuator::ActuatorMode((int)value));
        break;
    case 'p'://指定执行器位置，指令格式：p 圈数（-127到127）
        for (int i = 0; i < idArray.size(); ++i)
        {
            controllerInst->setPosition(idArray.at(i), value);
        }
        break;
    case 'c'://指定执行器电流，指令格式：c 电流值（A）
        for (int i = 0; i < idArray.size(); ++i)
        {
            controllerInst->setCurrent(idArray.at(i), value);
        }
        break;
    case 'v'://指定执行器速度，指令格式：v 速度值（RPM）
        for (int i = 0; i < idArray.size(); ++i)
        {
            controllerInst->setVelocity(idArray.at(i), value);
        }
        break;
    case 'l'://启动指定执行器，指令格式：l 执行器id（id为0启动所有执行器）
        if(uint8_t(value)==0)
        {
            controllerInst->launchAllActuators();
        }
        else
        {
            controllerInst->launchActuator(uint8_t(value));
        }
        //cout << "launch"<<endl;
        break;
    case 's'://关闭指定执行器，指令格式：l 执行器id（id为0启动所有执行器）
        if(uint8_t(value)==0)
        {
            controllerInst->closeAllActuators();
        }
        else
        {
            controllerInst->closeActuator(uint8_t(value));
        }
        //cout << "close"<<endl;
        break;
    default:
        break;
    }
}

void processSignal(int sign)
{
    //cin.clear();
    ActuatorController::getInstance()->closeAllActuators();
    //延时保证关机成功
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
}

int main(int argc, char *argv[])
{
    signal(SIGINT,processSignal);
    thread inputThread(keyboardInput);

    if(argc != 2)
    {
        std::cerr << "Usage: operateActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
        return 1;
    }

    //初始化控制器
    if(strcmp(argv[1],"-s")==0)
        ActuatorController::initController(argc,argv,Actuator::Via_Serialport);
    else if(strcmp(argv[1],"-e")==0)
        ActuatorController::initController(argc,argv);
    else
    {
        std::cerr << argv[1] <<"Usage: operateActuator <communicationType>\ncommunicationType:-s communicate via serialport or -e communicate via ethernet\n";
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
                    if(pController->getActuatorAttribute(id,Actuator::ACTUATOR_SWITCH)!=Actuator::ACTUATOR_SWITCH_OFF)
                    {
                        ++ nLaunchedActuatorCnt;
                        if(nLaunchedActuatorCnt == pController->getActuatorIdArray().size())//所有执行器都已启动完成
                        {
                            cout << "All actuators have launched!"  << endl;
                        }
                    }
                }

                break;
            }
            break;
        case Actuator::Launch_Finished:
            if(++nLaunchedActuatorCnt == pController->getActuatorIdArray().size())//所有执行器都已启动完成
            {
                cout << "All actuators have launched!"  << endl;
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
        if(directive != 0xff)
        {
            cmdOperation(directive,value/1000.0);
            directive = 0xff;
            value = 0;
        }
    }

    //断开信号连接
    pController->m_sOperationFinished->s_Disconnect(nOperationConnection);
    pController->m_sError->s_Disconnect(nErrorConnection);


    inputThread.join();
    //getchar();
    return 0;
}
