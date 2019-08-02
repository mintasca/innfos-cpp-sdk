/*
*查找已连接执行器
*/

#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string.h>
using namespace std;


int main(int argc, char *argv[])
{
    //Initialize the controller
    ActuatorController * pController = ActuatorController::initController();
    //ec Define an error type, ec==0x00 means no error, ec will be passed to pcontroller-> lookupActuators(ec) by reference,
    //when the error occurs, ec value will be modified by SDK to the corresponding error code
    Actuator::ErrorsDefine ec;
    //Find the connected actuators and return the UnifiedID of all actuators found.
    //UnifiedID is a structure composed of the actuator ID (actuatorID) and IP(ipAddress) of ECB(ECU)
    std::vector<ActuatorController::UnifiedID> uIDArray = pController->lookupActuators(ec);
    //If the size of the uIDArray is greater than zero, the connected actuators have been found
    if(uIDArray.size() > 0)
    {
        for(auto uID : uIDArray)
        {
            cout << "Actuator ID: "<<(int)uID.actuatorID << " IP address: " << uID.ipAddress.c_str() << endl;
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
