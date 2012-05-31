#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include "AbstractController.h"
#include "IntersectionController.h"
#include "Queue.h"

#include <vector>

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
    vector<Sensor *> sensors;
    int readFD;
    char sensorChar;

    //state related
    remoteState state;
    int mode;
    ControllerInfo::controllerState currentState;

public:
    RemoteController(char *machineName, char sensorChar);
    ~RemoteController();

    virtual void trigger() = 0;

    virtual void receiveMessage(char *sender, int header, int msg) = 0;
    virtual void clearFlag(unsigned int flag) = 0;
    virtual void setFlag(unsigned int flag) = 0;
    virtual int getFlag(unsigned int flag) = 0;

private:
    void createSensors();
};

#endif
