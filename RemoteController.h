#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include "AbstractController.h"
#include "IntersectionController.h"
#include "Sensor.h"
#include "Queue.h"

#include <vector>
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
        REQUEST_MODE
    } remoteHeader;

}

class RemoteController : AbstractController
{
private:
    //queue related
    char *machineName;
    Queue *incoming;
    unsigned int commandsToSend;

    //sensor related
    std::vector<Sensor *> sensors;
    char sensorChar;

    //state related
    RemoteInfo::remoteState state;
    int mode;
    ControllerInfo::controllerState currentState;

public:
    RemoteController(char *machineName, char sensorChar);
    ~RemoteController();

    virtual void trigger();

    virtual void receiveMessage(char *sender, int header, int msg);
    virtual void clearFlag(unsigned int flag);
    virtual void setFlag(unsigned int flag);
    virtual int getFlag(unsigned int flag);

    std::vector<Sensor *> *getSensors() { return &this->sensors; }

private:
    void createSensors();
};

#endif
