/*
*longId是64位长整形id标识执行器，包括通信单元id和短id，可以长短id相互转换
*/
#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string>
using namespace std;

bool bExit = false;
void processSignal(int sign)
{
    ActuatorController::getInstance()->closeAllActuators();
    //延时保证关机成功
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
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
    //关联控制器的longId操作信号
    int nOperationConnection = pController->m_sOperationFinishedL->s_Connect([&](uint64_t nDeviceId,uint8_t operationType){
        switch (operationType) {
        case Actuator::Recognize_Finished://自动识别完成
            if(pController->hasAvailableActuator())
            {
                //获取longId数组
                vector<uint64_t> longIdArray = pController->getActuatorLongIdArray();
                //获取byteid数组
                vector<uint8_t> idArray = pController->getActuatorIdArray();
                for (uint64_t id: longIdArray) {
                //获取长id中的通信ip地址
                    cout << "Communication IP is " << pController->toString(id) << endl;
                    //longId转换成byteId
                    cout << "Long id " << id << " convert to byte id " << (int)pController->toByteId(id) << endl;
                }
                for(uint8_t id : idArray)
                {
                    //byteId转换成longId
                    cout << "Byte id " << (int)id << " convert to long id " << pController->toLongId(id) << endl;
                }
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
