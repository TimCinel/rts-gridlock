#include <cstdlib>
#include <iostream>

#include "Sensor.h"

/*constructor*/
Sensor::Sensor(AbstractController *target, char trigger, int flagPosition,
    int readFD) : target(target), trigger(trigger), flagPosition(flagPosition),
    readFD(readFD), writeFD(0)
{
    this->createThread();
}

/*constructor where input stream is undefined*/
Sensor::Sensor(AbstractController *target, char trigger, int flagPosition) :
    target(target), trigger(trigger), flagPosition(flagPosition)
{
    int pipe_fds[2];

    /*create a pipe*/
    if (pipe(pipe_fds) != 0)
        throw "Failed to create pipe";
    this->readFD = pipe_fds[0];
    this->writeFD = pipe_fds[1];

    this->createThread();
}

/*accessor*/
int Sensor::getReadFD()
{
    return this->readFD;
}

/*accessor*/
int Sensor::getWriteFD()
{
    return this->writeFD;
}

/*accessor*/
int Sensor::getTrigger()
{
    return this->trigger;
}

/*create the sensor thread*/
void Sensor::createThread() {
    this->thread = new pthread_t;

    if (pthread_create(this->thread, NULL, sensor_util::listen, this) != 0)
        throw "Failed to create thread!";
}

/*deconstructor, cleans up after sensor is removed*/
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

/*triggers the key press by setting the applicable flag*/
void Sensor::checkTrigger(char received)
{
    if (this->trigger == received && this->target)
        this->target->setFlag(this->flagPosition);
}

/*non member function to run for thread. listens to a specific key as set in the
constructor and calls the trigger function*/
void *sensor_util::listen(void *args)
{
    char buff;
    Sensor *sensor = (Sensor *)args;

    while (read(sensor->getReadFD(), &buff, 1) > 0) {
        sensor->checkTrigger(buff);
    }

    return NULL;
}

