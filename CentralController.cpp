#include "CentralController.h"
#include "IntersectionController.h"

using namespace CentralInfo;

CentralController::CentralController(vector<string> intersectionNames)
{
    //create IntersectionRecords to maintain queues and state for
    //each intersection
    for (int i = 0; i < intersectionNames.size(); i++) {
        string name = intersectionNames[i];

        this->intersections[name] = IntersectionRecord(name.c_str(), this);
    }

    this->state = INITIALISE;

}

void CentralController::trigger()
{
    if (this->getTime() > 0)
        return;

    if ((this->stateRecord[this->state]) != NULL)
        (this->*stateRecord[this->state])();
    else
        (this->*stateRecord[STARTUP])();

}
void CentralController::receiveMessage(char *sender, int header, int msg)
{
}

void CentralController::clearFlag(unsigned int flag) 
{
    //TODO: Semaphore
    flags[flag % CENTRAL_FLAGS_SENTINEL] = 0;
}

void CentralController::setFlag(unsigned int flag)
{
    //TODO: Semaphore
    if (flag < CENTRAL_FLAGS_SENTINEL)
        flags[flag % CENTRAL_FLAGS_SENTINEL] = 1;
    else
        flags[SEND_COMMAND] = flag;
}

int CentralController::getFlag(unsigned int flag)
{
    //TODO: Semaphore
    return flags[flag % CENTRAL_FLAGS_SENTINEL]; 
}

void initialise(void)
{
    for (int i = 0; i < CENTRAL_FLAGS_SENTINEL; i++)
        this->clearFlag(0);

    //default state - notify controllers
    this->state = SEND_MODE;

    //default mode
    this->setFlag(CHANGE_TO_TIMER);
}

void listen(void)
{
}

void change_mode(void)
{
}
