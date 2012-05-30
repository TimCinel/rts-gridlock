#ifndef INTERSECTIONRECORD_H
#define INTERSECTIONRECORD_H

#include "IntersectionController.h"
#include "Queue.h"

class IntersectionRecord
{
public:
    IntersectionRecord(char *machineName, CentralController *controller);
    ~IntersectionRecord();

    ControllerInfo::controllerState getState();
    ControllerInfo::controllerState setState();

    char *getMachineName();
    Queue *getQueue();

private:
    char *machineName;

    ControllerInfo::controllerState state;
    Queue *incoming, *outgoing;
};

#endif
