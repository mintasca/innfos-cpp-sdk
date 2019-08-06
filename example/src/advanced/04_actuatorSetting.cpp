/*
*执行器参数设置
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
        if(pController->enableActuator(actuatorID))
        {
            cout << "Enable actuator " << (int)actuatorID << " successfully!" << endl;
        }
        //activate profile position mode
        pController->activateActuatorMode(actuatorID,Actuator::Mode_Profile_Pos);

        //change acceleration to 300 RPM/s
        pController->setProfilePositionAcceleration(actuatorID,300);
        //change deceleration to -300 RPM/s
        pController->setProfilePositionDeceleration(actuatorID,-300);
        //change max velocity to 500 RPM
        pController->setProfilePositionMaxVelocity(actuatorID,500);
        cout << "change position in low speed " << endl;
        pController->setPosition(actuatorID,-15);
        this_thread::sleep_for(std::chrono::seconds(5));


        //change acceleration to 1200 RPM/s
        pController->setProfilePositionAcceleration(actuatorID,1200);
        //change deceleration to -1200 RPM/s
        pController->setProfilePositionDeceleration(actuatorID,-1200);
        //change max velocity to 3000 RPM
        pController->setProfilePositionMaxVelocity(actuatorID,3000);
        cout << "change position in high speed " << endl;
        pController->setPosition(actuatorID,15);
        this_thread::sleep_for(std::chrono::seconds(4));

        //Save parameters,or you will lose all changes after disable the actuator
        if(pController->saveAllParams(actuatorID))
        {
            cout << "Save parameters sucessfully!" << endl;
        }

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
