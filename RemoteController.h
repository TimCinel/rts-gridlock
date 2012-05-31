#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include "AbstractController.h"
#include "IntersectionController.h"
#include "Sensor.h"
#include "Queue.h"

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

namespace RemoteInfo {
    typedef enum {
        LISTEN_ONLY,
        NOTIFY_AND_LISTEN,
        REMOTE_STATE_SENTINEL
    } remoteState;

    typedef enum {
        NOTIFY_STATE,
        NOTIFY_FLAGS,
        REQUEST_MODE
    } remoteHeader;

}

class RemoteController : public AbstractController
{
private:
    //MISC
        
    //the state that the RemoteController is currently in
    RemoteInfo::remoteState state; 
        
    //a bitstring of commands to send on the next trigger()
    //ControllerInfo::controllerFlag
    unsigned int commandsToSend;
        
        
    //QUEUE RELATED
        
    //path to message queue for IntersectionController
    const char *machineName;
    Queue *incoming;


    //STATE RELATED
    
    //the mode that intersection should be in:
    //COMMAND_MODE, SENSOR_MODE, TIMER_MODE (ControllerInfo::controllerFlag)
    int mode; 

    //the state that the intersection last reported
    ControllerInfo::controllerState remoteState;

    //the flags that thae intersection last reported
    int remoteFlags;

public:
    RemoteController(const std::string &machineName);
    ~RemoteController();

    //AbstractController methods

    //send all commands set in commandsToSend
    virtual void trigger();

    virtual void receiveMessage(char *sender, int header, int msg);
    virtual void clearFlag(unsigned int flag);
    virtual void setFlag(unsigned int flag);
    virtual int getFlag(unsigned int flag);

private:
    void createSensors();
};

#endif
