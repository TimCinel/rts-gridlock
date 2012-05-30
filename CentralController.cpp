#include "CentralController.h"

void CentralController::trigger()
{
    if (this->getTime() > 0)
        return;

    if ((this->stateRecord[this->state]) != NULL)
        (this->*stateRecord[this->state])();
    else
        (this->*stateRecord[STARTUP])();

}

void CentralController::clearFlag(unsigned int flag) 
{

}

void CentralController::setFlag(unsigned int flag)
{

}

int CentralController::getFlag(unsigned int flag)
{

}
