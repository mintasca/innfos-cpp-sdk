/*
*批量使能执行器
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
    //Initialize the controller
    ActuatorController * pController = ActuatorController::initController();
    //ec Define an error type, ec==0x00 means no error, ec will be passed to pcontroller-> lookupActuators(ec) by reference,
    //when the error occurs, ec value will be modified by SDK to the corresponding error code
    Actuator::ErrorsDefine ec;
    //Find the connected actuators and return the UnifiedID of all actuators found.
    std::vector<ActuatorController::UnifiedID> uIDArray = pController->lookupActuators(ec);
    //If the size of the uIDArray is greater than zero, the connected actuators have been found
    if(uIDArray.size() > 0)
    {
        //Enable all connected actuators
        if(pController->enableActuatorInBatch(uIDArray))
        {
            cout << "All actuators have been enabled successfully! " << endl;
        }
        //Disable all connected actuators
        pController->disableAllActuators();
        //insure that all actuators have been closed
        this_thread::sleep_for(std::chrono::milliseconds(200));

    }
    else
    {
        cout << "Connected error code:" << hex << ec << endl;
    }

    return 0;
}
