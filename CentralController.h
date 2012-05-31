#ifndef CENTRALCONTROLLER_H
#define CENTRALCONTROLLER_H

#include "AbstractController.h"
#include "IntersectionRecord.h"

namespace CentralInfo {

    typedef enum {
        INITIALISE,
        LISTEN,
        SEND_MODE,
        CENTRAL_STATES_SENTINEL
    } centralState;
}

class CentralController : public AbstractController
{
public:
    //constructor
    CentralController(vector<string> intersectionNames);
    virtual ~CentralController();

    //functions
    virtual void trigger() = 0;
    void tick();

    virtual void receiveMessage(char *sender, int header, int msg) = 0;
    virtual void clearFlag(unsigned int flag) = 0;
    virtual void setFlag(unsigned int flag) = 0;
    virtual int getFlag(unsigned int flag) = 0;

private:
    //instance variables
    CentralInfo::centralState state;
    void (CentralController::*stateRecord
            [CentralInfo::CENTRAL_STATES_SENTINEL])();

    unsigned int flags[ControllerInfo::CONTROLLER_FLAG_SENTINEL];
    map<string, IntersectionRecord> intersections;

};

#endif
