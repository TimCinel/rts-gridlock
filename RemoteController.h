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

/*contains the headers for messages to the remote controller*/
namespace RemoteInfo
{
    typedef enum {
        REQUEST_MODE,
        NOTIFY_STATE,
        NOTIFY_FLAGS,
        NOTIFY_LIGHTS_NS,
        NOTIFY_LIGHTS_EW
    } remoteHeader;

    static const int MAX_NAME_LEN = 128;
}

class RemoteController : public AbstractController
{
    /*instance variables*/
    private:
        /*bitstrings of commands to set/clear on the next trigger()
        bit positions represetn values of ControllerInfo::controllerFlag*/
        unsigned int flagsToSet;
        unsigned int flagsToClear;
        
        /*path to message queue for IntersectionController*/
        char *centralName;
        char *intersectionName;
        Queue *incoming;

        /*the mode that intersection should be in: COMMAND_MODE, SENSOR_MODE,
        TIMER_MODE (ControllerInfo::controllerFlag)*/
        int mode; 

        /*the state that the intersection last reported*/
        ControllerInfo::controllerState remoteState;

        /*the flags and lights that thae intersection last reported*/
        int remoteFlags, remoteLightsNS, remoteLightsEW;

    /*constructors*/
    public:
        RemoteController(std::string centralName, std::string intersectionName);
        ~RemoteController();

    /*member functions*/
    public:
        virtual void trigger();

        virtual void receiveMessage(char *sender, int header, int msg);
        virtual void clearFlag(unsigned int flag);
        virtual void setFlag(unsigned int flag);
        virtual int getFlag(unsigned int flag);

        virtual void display();

        char* getCentralName();
        char* getIntersectionName();
};

#endif

