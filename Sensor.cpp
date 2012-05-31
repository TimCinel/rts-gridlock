#include <cstdlib>
#include <iostream>

#include "Sensor.h"

Sensor::Sensor() {
    throw "Specify target, trigger, and flagPosition when instantiating Sensor";
}

Sensor::Sensor(AbstractController *target, char trigger, int flagPosition, int readFD) :
        target(target), trigger(trigger), flagPosition(flagPosition), 
        readFD(readFD), writeFD(0)
{
    this->createThread();
}

Sensor::Sensor(AbstractController *target, char trigger, int flagPosition) :
    target(target), trigger(trigger), flagPosition(flagPosition)
{
    int pipe_fds[2];

    //create a pipe
    if (pipe(pipe_fds) != 0)
        throw "Failed to create pipe";

    this->readFD = pipe_fds[0];
    this->writeFD = pipe_fds[1];

    this->createThread();
}

void Sensor::createThread() {
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

    if (this->writeFD)
        close(writeFD);

    if (this->readFD)
        close(readFD);

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

    while (read(sensor->getReadFD(), &buff, 1) > 0) {
        sensor->checkTrigger(buff);
    }

    return NULL;
}
