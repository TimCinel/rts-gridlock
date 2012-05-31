#include "RemoteController.h"

using namespace RemoteInfo;
using namespace ControllerInfo;

RemoteController::RemoteController(const std::string &centralName, 
                                   const std::string &intersectionName)
{
    this->centralName = centralName.c_str();
    this->intersectionName = intersectionName.c_str();

    //create a Queue and receive messages from the intersection
    this->incoming = NULL;
    this->incoming = new Queue((char *)this->centralName, this);

}

RemoteController::~RemoteController()
{
    if (this->incoming)
        delete this->incoming;
}

void RemoteController::trigger() {
    char *dest = (char *)this->intersectionName;
    char *nullString = (char *)"";
    
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
            write_queue(dest, SET_CONTROLLER_FLAG, nullString, i);
        if (1 & (flagsToClear >> i)) 
            write_queue(dest, CLEAR_CONTROLLER_FLAG, nullString, i);
    }
}

void RemoteController::receiveMessage(char *sender, int header, int msg)
{
    switch (header) {
        case REQUEST_MODE:
            //intersection has requested that the current mode is sent to it

            this->setFlag(this->mode);

            break;

        case NOTIFY_STATE:
            //remote intersection has sent its state

            std::cout << this->intersectionName << " state: " << 
                          controllerStateNames[msg % CONTROLLER_STATE_SENTINAL]  
                          << "\n";
            this->remoteState = (controllerState)msg;

            break;

        case NOTIFY_FLAGS:
            //remote intersection has sent its flags

            std::cout << this->intersectionName << " flags: " << msg << "\n";
            this->remoteFlags = msg;

            break;

        case NOTIFY_LIGHTS_NS:
            //remote intersection has sent its NS lights

            this->remoteLightsNS = msg;

            break;
        case NOTIFY_LIGHTS_EW:
            //remote intersection has sent its EW lights

            this->remoteLightsEW = msg;

            break;

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

void RemoteController::display()
{
    //TODO: Implement this
    std::cout << "IMPLEMENT ME!\n";
}
