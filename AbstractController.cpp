#include "AbstractController.h"

#include <iostream>

void AbstractController::tick()
{
    while (1)
    {
        while (!getWaynesConstant());
        //std::cout << "consumerdown...";
        downMutex();

        this->time--;
        std::cout << "Time: " << this->time << "\n";
        this->trigger();

        //std::cout << "consumerup!";
        upMutex();
        clearWaynesConstant();
    }
}

void AbstractController::initClock()
{
    mutex = PTHREAD_MUTEX_INITIALIZER;
    waynesConstant = 1;

    pthread_create(&timer, NULL, runClock, this);
}

void* runClock(void* ptr)
{
    AbstractController* controller = (AbstractController*) ptr;

    while (1)
    {
        sleep(1);

        while (controller->getWaynesConstant());
        //std::cout << "producerdown...";
        controller->downMutex();

        //std::cout << "producerup!";
        controller->upMutex();
        controller->setWaynesConstant();
    }

    return 0;
}

void AbstractController::endClock()
{
    void* result;

    pthread_join(timer, &result);

    //do something with result?
}

void AbstractController::downMutex()
{
    pthread_mutex_lock(&mutex);
}

void AbstractController::upMutex()
{
    pthread_mutex_unlock(&mutex);
}

int AbstractController::getWaynesConstant()
{
    return waynesConstant;
}

void AbstractController::setWaynesConstant()
{
    waynesConstant = 1;
}

void AbstractController::clearWaynesConstant()
{
    waynesConstant = 0;
}

void AbstractController::toggleWaynesConstant()
{
    waynesConstant = !waynesConstant;
}

