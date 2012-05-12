#include <iostream>
#include "Sensor.h"

Sensor::Sensor() {
    throw "Specify target, trigger, and flagPosition when instantiating Sensor";
}

Sensor::Sensor(Controller *target, char trigger, int flagPosition, int readFd) :
        target(target), trigger(trigger), flagPosition(flagPosition), 
        readFd(readFd) {

    std::cout << "Creating....\n";

    this->thread = new pthread_t;

    //spin up a new thread    
    if (pthread_create(this->thread, NULL, sensor_util::listen, this) != 0)
        throw "Failed to create thread!";

    std::cout << "Created.\n";

}

Sensor::~Sensor() {
    if (NULL != this->thread)
        pthread_cancel(*this->thread);

    if (this->thread)
        delete(this->thread);

    if (this->threadAttr)
        delete(this->threadAttr);

}

void Sensor::checkTrigger(char received) {
    if (this->trigger == received && this->target)
        this->target->setSensorFlag(this->flagPosition);
}


void *sensor_util::listen(void *args) {
    char buff;
    Sensor *sensor = (Sensor *)args;

    std::cout << "Listening for " << sensor->getTrigger() << "!\n";

    while (read(sensor->getReadFd(), &buff, 1) > 0) {
        sensor->checkTrigger(buff);
    }

    std::cout << "Done Listening for " << sensor->getTrigger() << "!\n";
}
