#include "AbstractController.h"

void AbstractController::tick()
{
    while (1)
    {
        while (!waynesConstant);
        std::cerr << "consumerdown...";
        pthread_mutex_lock(&mutex);

        this->time--;
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
        this->trigger();
        display();
        std::cout << "Time: " << this->time << "\n";

        std::cerr << "consumerup!";
        pthread_mutex_unlock(&mutex);
        waynesConstant = 0;
    }
}

void AbstractController::resetTimer(int time)
{
    this->time = time;
}

int AbstractController::getTime()
{
    return this->time;
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
        std::cerr << "producerdown...";
        pthread_mutex_lock(controller->getMutex());

        std::cerr << "producerup!";
        pthread_mutex_unlock(controller->getMutex());
        controller->setWaynesConstant(1);
    }

    return 0;
}

void AbstractController::endClock()
{
    void* result;

    pthread_join(timer, &result);

    //do something with result?
}

pthread_mutex_t* AbstractController::getMutex()
{
    return &mutex;
}

int AbstractController::getWaynesConstant()
{
    return waynesConstant;
}

void AbstractController::setWaynesConstant(int n)
{
    waynesConstant = n;
}

