#ifndef CENTRALCONTROLLER_H
#define CENTRALCONTROLLER_H

#include "AbstractController.h"
#include "IntersectionRecord.h"

namespace CentralInfo {

    typedef enum {
        INITIALISE,
        LISTEN,
        CHANGE_MODE,
        CENTRAL_STATES_SENTINEL
    } CENTRAL_STATES;
}

class CentralController : public AbstractController
{
public:
    //constructor
    CentralController();
    virtual ~CentralController();

    //functions
    virtual void trigger() = 0;
    void tick();

    virtual void clearFlag(unsigned int flag) = 0;
    virtual void setFlag(unsigned int flag) = 0;
    virtual int getFlag(unsigned int flag) = 0;

private:
    //instance variables

    unsigned int flags[ControllerInfo::CONTROLLER_FLAG_SENTINEL];
    void (CentralController::*stateRecord
            [CentralInfo::CENTRAL_STATES_SENTINEL])();

    vector<IntersectionRecord> 

};

#endif
