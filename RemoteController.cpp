#include "RemoteController.h"

using namespace RemoteInfo;
using namespace ControllerInfo;

RemoteController::RemoteController(const std::string &machineName) 
{
    this->machineName = machineName.c_str();

    this->incoming = NULL;
    this->incoming = new Queue((char *)this->machineName, this);

}

RemoteController::~RemoteController()
{
    if (this->incoming)
        delete this->incoming;
}

void RemoteController::trigger() {
    char *machineName = (char *)this->machineName;
    char *nullString = (char *)"";
    
    if (NOTIFY_AND_LISTEN == this->state) {
        //set a flag so the mode will be sent on next trigger
        this->setFlag(this->mode);

        //just listen next time
        this->state = LISTEN_ONLY;
    }

    //TODO: Semaphore down
    
    //copy flags they arent' held for long
    int flagsToSet = this->flagsToSet;
    int flagsToClear = this->flagsToClear;

    //reset flags
    this->flagsToSet = 0;
    this->flagsToClear = 0;

    //TODO: Semaphore up
    
    //rotate through send messages to set and clear flags 
    for (int i = 0; i < CONTROLLER_MODE_SENTINEL; i++)
    {
        if (1 & (flagsToSet >> i)) 
            write_queue(machineName, SET_CONTROLLER_FLAG, nullString, i);
        if (1 & (flagsToClear >> i)) 
            write_queue(machineName, CLEAR_CONTROLLER_FLAG, nullString, i);
    }
}

void RemoteController::receiveMessage(char *sender, int header, int msg)
{
    if (NOTIFY_STATE == header)
    {
        //remote intersection has sent its state
        std::cout << this->machineName << " state: " << 
                      controllerStateNames[msg % CONTROLLER_STATE_SENTINAL]  <<
                      "\n";
        this->remoteState = (controllerState)msg;

    }
    else if (NOTIFY_FLAGS == header) 
    {
        //remote intersection has sent its flags
        std::cout << this->machineName << " flags: " << msg << "\n";
        this->remoteFlags = msg;

    }
    else if (REQUEST_MODE == header)
    {
        //on next tick, notify 
        this->setFlag(this->mode);
    }
}

void RemoteController::clearFlag(unsigned int flag)
{
    //TODO: Semaphore down
    //NOTE: this SETS the bit in flagsToClear
    this->flagsToClear &= (1 << flag);
    //TODO: Semaphore up

    std::cout << "The flag `" << controllerFlagNames[flag] << "` has been cleared\n";
}

void RemoteController::setFlag(unsigned int flag)
{
    //TODO: Semaphore down
    //sets bit in flagsToSet
    this->flagsToSet &= (1 << flag);
    //TODO: Semaphore up

    std::cout << "The flag `" << controllerFlagNames[flag] << "` has been set\n";

    //the flag being set is a mode-setting one, update 'mode' accordingly
    if (flag > CONTROLLER_FLAG_SENTINEL && flag < CONTROLLER_MODE_SENTINEL)
    {
        std::cout << "The mode has been changed to " << controllerFlagNames[flag] << "\n";
        this->mode = flag;
    }

}

int RemoteController::getFlag(unsigned int flag)
{
    //doesn't do anything
    return 0;
}
