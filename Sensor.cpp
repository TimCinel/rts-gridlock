#include <iostream>
#include "Sensor.h"

Sensor::Sensor() {
    throw "Specify target, trigger, and flagPosition when instantiating Sensor";
}

Sensor::Sensor(AbstractController *target, char trigger, int flagPosition, int readFd) :
        target(target), trigger(trigger), flagPosition(flagPosition), 
        readFd(readFd) {

    this->thread = new pthread_t;

    //spin up a new thread    
    if (pthread_create(this->thread, NULL, sensor_util::listen, this) != 0)
        throw "Failed to create thread!";
}

Sensor::~Sensor()
{
    if (NULL != this->thread)
        pthread_cancel(*this->thread);

    if (this->thread)
        delete(this->thread);

    if (this->threadAttr)
        delete(this->threadAttr);

}

void Sensor::checkTrigger(char received)
{
    if (this->trigger == received && this->target)
        this->target->setFlag(this->flagPosition);
}


void *sensor_util::listen(void *args)
{
    char buff;
    Sensor *sensor = (Sensor *)args;

    while (read(sensor->getReadFd(), &buff, 1) > 0) {
        sensor->checkTrigger(buff);
    }
}
