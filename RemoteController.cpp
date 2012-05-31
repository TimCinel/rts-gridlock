#include "RemoteController.h"

using namespace RemoteInfo;
using namespace ControllerInfo;

RemoteController::RemoteController(char *machineName, char sensorChar) 
{
    this->machineName = machineName;

    this->incoming = NULL;
    this->incoming = new Queue(this->machineName, this);

    this->createSensors();

}

RemoteController::~RemoteController()
{
    if (this->incoming)
        delete this->incoming;
}

void RemoteController::trigger() {
    
    if (NOTIFY_AND_LISTEN == this->state) {
        //set a flag so the mode will be sent on next trigger
        this->setFlag(this->mode);

        //just listen next time
        this->state = LISTEN_ONLY;
    }

    for (int i = ControllerInfo::CONTROLLER_COMMAND_SENTINEL; 
             i < ControllerInfo::CONTROLLER_MODE_SENTINEL; i++)
    {
        if (getFlag(i)) 
        {
            write_queue(
                        this->machineName, 
                        ControllerInfo::SET_CONTROLLER_FLAG, 
                        (char *)"", 
                        i
                    );

            this->clearFlag(i);
        }
    }
}

void RemoteController::receiveMessage(char *sender, int header, int msg)
{
    if (NOTIFY_STATE == header)
    {
        //intersection has changed state

        std::cout << this->machineName << ": " << 
                      controllerStateNames[msg % CONTROLLER_STATE_SENTINAL]  <<
                      "\n";

        this->currentState = (controllerState)msg;
    }
    else if (REQUEST_MODE == header)
    {
        //on next tick, notify 

        this->state = NOTIFY_AND_LISTEN;
    }
}

void RemoteController::clearFlag(unsigned int flag)
{
    //TODO: Semaphore
    this->commandsToSend &= ~(1 << flag);
}

void RemoteController::setFlag(unsigned int flag)
{
    //TODO: Semaphore
    this->commandsToSend &= (1 << flag);

    std::cout << "The flag `" << controllerStateNames[flag] << "` has been set\n";

    //the user has set a new mode
    if (flag > CONTROLLER_FLAG_SENTINEL &&
        flag < CONTROLLER_MODE_SENTINEL)
    {
        std::cout << "The mode has been changed to " << controllerStateNames[flag] << "\n";
        this->mode = flag;
    }

}

int RemoteController::getFlag(unsigned int flag)
{
    //TODO: Semaphore
    return 1 & (this->commandsToSend >> flag);
}

void RemoteController::createSensors()
{

    for (int i = CONTROLLER_MODE_SENTINEL - 1; 
             i > CONTROLLER_COMMAND_SENTINEL; 
             i--) 
    {
        if (i == CONTROLLER_FLAG_SENTINEL)
            continue;

        std::cout << "Press " << (char)this->sensorChar++ << " for " <<
                     controllerFlagNames[i] << "\n";

        this->sensors.push_back(new Sensor(this, this->sensorChar, i));
    }
}
