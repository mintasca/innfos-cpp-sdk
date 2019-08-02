/*
*异步回调
*/

#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string.h>
#include <chrono>
using namespace std;

bool bExit = false;

void processSignal(int sign)
{
    ActuatorController::getInstance()->disableAllActuators();
    this_thread::sleep_for(std::chrono::milliseconds(200));
    bExit = true;
}

//
void paramFeedback(ActuatorController::UnifiedID uID,uint8_t paramType,double paramValue)
{
    switch (paramType) {
    case Actuator::ACTUAL_CURRENT:
        cout << "Actuator " << (int)uID.actuatorID << " current is " << paramValue << "A"<<endl;
        break;
    case Actuator::ACTUAL_POSITION:
        cout << "Actuator " << (int)uID.actuatorID << " position is " << paramValue << "R"<<endl;
        break;
    case Actuator::ACTUAL_VELOCITY:
        cout << "Actuator " << (int)uID.actuatorID << " velocity is " << paramValue << "RPM"<<endl;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    //Associate program interrupt signals and call processSignal when you end the program with ctrl-c
    signal(SIGINT,processSignal);
    //Initialize the controller
    ActuatorController * pController = ActuatorController::initController();
    //ec Define an error type, ec==0x00 means no error, ec will be passed to pcontroller-> lookupActuators(ec) by reference,
    //when the error occurs, ec value will be modified by SDK to the corresponding error code
    Actuator::ErrorsDefine ec;
    //Find the connected actuators.
    pController->lookupActuators(ec);
    //Gets an array of all actuator IDs
    vector<uint8_t> idArray = pController->getActuatorIdArray();
    //If the size of the idArray is greater than zero, the connected actuators have been found
    if(idArray.size() > 0)
    {
        //Enable actuator
        if(pController->enableActuator(idArray.at(0)))
        {
            cout << "Enable actuator " << (int)idArray.at(0) << " successfully!" << endl;
        }

        //Add an actuator request parameter callback which will be invoked when the parameter request returns
        pController->addParaRequestCallback(paramFeedback);

        while (!bExit) {
            //Event polling, polling callback events, event triggering calls to the corresponding callback function
            ActuatorController::processEvents();
            //Asynchronous request executor current, velocity, poistion, and when the request returns,
            //the callback function is triggered by a polling callback event. This function does not block.
            pController->requestCVPValue(idArray.at(0));

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    else
    {
        //ec=0x803 Communication with ECB(ECU) failed
        //ec=0x802 Communication with actuator failed
        cout << "Connected error code:" << hex << ec << endl;
    }

    return 0;
}
