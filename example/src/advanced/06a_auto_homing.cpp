/*
*执行器手动调零
*/

#include <iostream>
#include "actuatorcontroller.h"
#include <thread>
#include <signal.h>
#include <string.h>
#include <chrono>
using namespace std;

#define SCA_POS_ZERO 1e-5
#define SCA_POS_MIN_ERR 0.05
#define SCA_POS_ERR_RATIO 1 // percent
#define SCA_HOMING_ZERO 0.05
#define SCA_CURRENT_ZERO 1e-3
#define SCA_CURRENT_SAFE 10.0

#define SLEEP_MS(X) \
  do {\
  	std::this_thread::sleep_for(std::chrono::milliseconds(X));\
  } while(0)

const double HOMING_POS_LIMIT_MAX = 100.0;
const double HOMING_POS_LIMIT_MIN = -100.0;
const double HOMING_VELOCITY = 50.0;
const double THRESHOLD_CURRENT[6] = {4.0, 5.0, 3.0, 3.0, 2.0, 2.0};
const double POS_MARGIN = 0.05;
const unsigned int PERIOD_HOMING = 100;
const int MAX_TIME_COUNT = 1000;

bool AutoHoming(ActuatorController * pController, uint8_t id, bool bParam = true);
void MoveToPos(ActuatorController * pController, uint8_t id, double pos, bool checkWait = false);

int main(int argc, char *argv[])
{
	  printf("main start ......\n");
    //Initialize the controller
    ActuatorController * pController = ActuatorController::initController();
    //ec Define an error type, ec==0x00 means no error, ec will be passed to pcontroller-> lookupActuators(ec) by reference,
    //when the error occurs, ec value will be modified by SDK to the corresponding error code
    Actuator::ErrorsDefine ec;
    //Find the connected actuators and return the UnifiedID of all actuators found.
    std::vector<ActuatorController::UnifiedID> uIDArray = pController->lookupActuators(ec);
    //If the size of the uIDArray is greater than zero, the connected actuators have been found
    if(uIDArray.size() == 6)
    {
        for(int i = 0; i < uIDArray.size(); ++i) {
          //Enable an actuator,If there are no actuators with the same ID under multiple IP addresses, you can omit the ipAddress parameter
          pController->enableActuator(uIDArray.at(i).actuatorID, uIDArray.at(i).ipAddress);
          printf("SCA ID[%d]: enable\n", uIDArray.at(i).actuatorID);
          SLEEP_MS(500);
          //activate profile position mode
          pController->activateActuatorMode(uIDArray.at(i).actuatorID, Actuator::Mode_Profile_Pos);
        }
        //auto check 1 2 actuator
        if (!AutoHoming(pController, uIDArray.at(2).actuatorID, false)) {
        	printf("main exit: SCA ID[%d] exception, please check sca status by IAS Tool ......\n", uIDArray.at(2).actuatorID);
        	return 0;
        }
        if (!AutoHoming(pController, uIDArray.at(1).actuatorID, false)) {
        	printf("main exit: SCA ID[%d] exception, please check sca status by IAS Tool ......\n", uIDArray.at(1).actuatorID);
        	return 0;
        }
        //auto homing 0 1 2 3 4 5 actuator
        for(int i = uIDArray.size(); i > 0; --i) {
          if (!AutoHoming(pController, uIDArray.at(i-1).actuatorID)) {
           	printf("main exit: SCA ID[%d] exception, please check sca status by IAS Tool ......\n", uIDArray.at(i-1).actuatorID);
        	  return 0;
          }
        }
        //insure that all actuators have been closed
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    else
    {
        cout << "Connected error code:" << hex << ec << endl;
    }
    
    printf("main exit ......\n");

    return 0;
}

bool AutoHoming(ActuatorController * pController, uint8_t id, bool bParam /*=true*/)
{
    printf("SCA ID[%d]: auto homing, begin...\n", id);

    // first we need open position max/min for homing in case home pos lost
    pController->activateActuatorMode(id, Actuator::Mode_Profile_Pos);
    pController->setMaximumPosition(id, HOMING_POS_LIMIT_MAX);
    pController->setMinimumPosition(id, HOMING_POS_LIMIT_MIN);
    
    // set profile velocity mode
    pController->activateActuatorMode(id, Actuator::Mode_Profile_Vel);

    double vel = 200; // HOMING_VELOCITY;
    double pos1 = 0;
    double pos1_current = 0;
    double pos2 = 0;
    double data_Current_Real = 0.0;
    double data_Position_Real = 0.0;
    
    pController->setVelocity(id, vel);
    while (1)
    {
        data_Current_Real = pController->getCurrent(id, true);
        printf(
            "SCA ID[%d]: current=%.1f/%.1f, vel=%.1f, pos1=%.2f, pos2=%.2f, "
            "pos1_current=%.2f\n",
            id, data_Current_Real, THRESHOLD_CURRENT[id-1], vel, pos1, pos2,
            pos1_current);
        if (abs(data_Current_Real) > SCA_CURRENT_SAFE) {
        	  printf("SCA ID[%d]: current over safe threshold\n", id);
        	  pController->setVelocity(id, 0);
        	  pController->activateActuatorMode(id, Actuator::Mode_Profile_Pos);
        	  return false;
        }
        if (abs(data_Current_Real) >= THRESHOLD_CURRENT[id-1])
        {
            data_Position_Real = pController->getPosition(id, true);
            printf("SCA ID[%d]: Current up to threshold %.1f, get a pos limit %.2f\n",
                  id, THRESHOLD_CURRENT[id-1], data_Position_Real);

            // first fill max1, then max2
            if (abs(pos1) <= SCA_POS_ZERO)
            {
                pos1 = data_Position_Real;
                pos1_current = data_Current_Real;
                printf(
                    "SCA ID[%d]: current up to threshold %.1f, get pos1 = %.2f, turn "
                    "around...\n",
                    id, THRESHOLD_CURRENT[id-1], pos1);
                vel = -vel;
                pController->setVelocity(id, vel);
            }
            else
            {
                pos2 = data_Position_Real;
                printf(
                    "SCA ID[%d]: current up to threshold %.1f, get pos2 = %.2f, "
                    "homing over.\n",
                    id, THRESHOLD_CURRENT[id-1], pos2);
                break;
            }
        }
        SLEEP_MS(PERIOD_HOMING);
    }
    pController->setVelocity(id, 0);

    // do set homing with pos1/2 detected
    printf("SCA ID[%d]: set homing by pos limit (%f, %f)\n", id, pos2, pos1);
    double zero_pos = (pos1 + pos2) / 2;
    double limit = abs(pos1 - zero_pos) - POS_MARGIN;
    
    // NOTE: because after clear homing, acutator will take current pos as
    // zero so we must move acutator to zero pos first
    MoveToPos(pController, id, zero_pos, true); // need to wait move to pos finished
    
    if (bParam) {
    	  printf("SCA ID[%d]: setHoming zero_pos %.4f, limit %.4f / %.4f\n",
               id, zero_pos, limit, -limit);
        pController->activateActuatorMode(id, Actuator::Mode_Homing);
        pController->clearHomingInfo(id);
    	  pController->setHomingPosition(id, zero_pos);
    	  pController->setMaximumPosition(id, limit);
        pController->setMinimumPosition(id, -limit);
        pController->saveAllParams(id);
    
        printf("SCA ID[%d]: move sca back to zero with new params.\n", id);
        MoveToPos(pController, id, 0.0);
    }
    
    printf("SCA ID[%d]: auto homing, end.\n", id);
    return true;
}

void MoveToPos(ActuatorController * pController, uint8_t id, double pos, bool checkWait /*=false*/)
{
	  Actuator::ActuatorMode current_mode;
	  while(1) {
        pController->activateActuatorMode(id, Actuator::Mode_Profile_Pos);
        current_mode =	pController->getActuatorMode(id);
      	if (Actuator::Mode_Profile_Pos == current_mode) {
      		break;
      	} else {
      		printf("MoveToPos SCA ID[%d]: set Mode_Profile_Pos failed, current_mode %d", id, current_mode);
      	}
    }
    printf("MoveToPos SCA ID[%d]: pos %.4f, current_mode %d.\n", id, pos, current_mode);
	  pController->setPosition(id, pos);
    if (checkWait)
    {
        int time_count = 0;
        const int MAX_TIME_COUNT = 1000;
        double data_Position_Real = 0.0;
        
        while (1)
        {
            SLEEP_MS(100);
            data_Position_Real = pController->getPosition(id, true);
            if (abs(data_Position_Real - pos) <= SCA_POS_MIN_ERR)
            {
                printf("SCA ID[%d]: move to pos %f, current %f, success\n",
                      id, pos, data_Position_Real);
                break;
            }
            else
            {
                printf("SCA ID[%d]: move to pos %f, current %f...\n",
                      id, pos, data_Position_Real);
                time_count++;
                if (time_count >= MAX_TIME_COUNT)
                {
                    printf("SCA ID[%d]: time count up to max, stop wait\n", id);
                    break;
                }
            }
        }
    }
}