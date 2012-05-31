#include "RemoteController.h"

RemoteController::RemoteController(char *machineName, 
        CentralController *controller) 
{
    this->machineName = machineName;
    this->controller = controller;

    this->incoming = NULL;
    this->incoming = new Queue(this->machineName, this->controller);

    this->createSensors();

}

~RemoteController()
{
    if (this->incoming)
        delete this->incoming;
}

virtual void trigger() {
    
    if (NOTIFY_AND_LISTEN == this->state) {

        //send a command to set the flag for the current mode
        SendMessage(this->machineName, 
                    ControllerInfo::SET_CONTROLLER_FLAG, 
                    this->mode);

        //just listen next time
        this->state = LISTEN_ONLY;
    }

    for (int i = ControllerInfo::CONTROLLER_COMMAND_SENTINEL; 
             i < ControllerInfo::CONTROLLER_MODE_SENTINEL; i++)
    {
        if (getFlag(i)) 
        {
            SendMessage(this->machineName, 
                        ControllerInfo::SET_CONTROLLER_FLAG, i);
            this->clearFlag(i);
        }
    }
}

virtual void receiveMessage(char *sender, int header, int msg)
{
    if (NOTIFY_STATE == header)
    {
        //intersection has changed state

        std::count << this->machineName << ": " << 
                      controllerStateNames[mag % CONTROLLER_STATE_SENTINAL]  <<
                      "\n";

        this->currentState = msg;
    }
    else if (REQUEST_MODE == header)
    {
        //on next tick, notify 

        this->state = NOTIFY_AND_LISTEN;
    }
}

virtual void clearFlag(unsigned int flag)
{
    //TODO: Semaphore
    this->commandsToSend &= ~(1 << flag);
}

virtual void setFlag(unsigned int flag)
{
    //TODO: Semaphore
    this->sendCommands &= (1 << flag);

    //the user has set a new mode
    if (flag > CONTROLLER_FLAG_SENTINEL &&
        flag < CONTROLLER_MODE_SENTINEL)
        this->mode = flag;

}

virtual int getFlag(unsigned int flag)
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

        std::cout << "Press " << (char)this->sensorChar << " for " <<
                     centralFlagNames[i] << "\n";

        this->sensors.push_back(new Sensor(this, this->sensorChar, i));
    }
}
