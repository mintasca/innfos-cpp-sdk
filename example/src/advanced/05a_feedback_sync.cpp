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
        uint8_t actuatorID = idArray.at(0);
        //Enable actuator
        if(pController->enableActuator(actuatorID))
        {
            cout << "Enable actuator " << (int)idArray.at(0) << " successfully!" << endl;
        }
        //if obtain parameters of the actuator synchronously,there is some communication blocking time
        /**
         * Read the position of the actuator, if the second parameter is true, sdk will send the read position request to the actuator and wait for the return,
         * otherwise, the result of the last request is returned immediately
         **/
        double pos = pController->getPosition(actuatorID,true);
        /**
         * Read the current of the actuator, if the second parameter is true, sdk will send the read current request to the actuator and wait for the return,
         * otherwise, the result of the last request is returned immediately
         **/
        double cur = pController->getCurrent(actuatorID,true);
        cout << "Actuator postion:" << pos << "R,current:" << cur << "A" <<endl;


        pController->disableActuator(idArray.at(0));
        //insure that the actuator has been closed
        this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    else
    {
        //ec=0x803 Communication with ECB(ECU) failed
        //ec=0x802 Communication with actuator failed
        cout << "Connected error code:" << hex << ec << endl;
    }

    return 0;
}
