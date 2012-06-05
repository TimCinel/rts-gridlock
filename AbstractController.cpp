#include "AbstractController.h"

/*updates the counter approximatley once every second by using the shared mutex
which is set by the runClock function which operates in a seperate thread*/
void AbstractController::tick()
{
    /*loop forever, only ends when intersection is shut off*/
    while (1)
    {
        while (!waynesConstant);
        /*std::cerr << "consumerdown...";*/
        /*pthread_mutex_lock(&mutex);*/

        /*decrement the timer, counting towards 0*/
        this->time--;

        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n";

        /*when the timer reaches 0, the trigger function will cause state
        transition*/
        this->trigger();

        display();
        std::cout << "Time: " << this->time << "\n";

        /*std::cerr << "consumerup!";*/
        /*pthread_mutex_unlock(&mutex);*/
        waynesConstant = 0;
    }
}

/*sets the timer whenever a new state is entered*/
void AbstractController::resetTimer(int time)
{
    this->time = time;
}

/*accessor*/
int AbstractController::getTime()
{
    return this->time;
}

/*creates the clock thread, MUST be called at intersection or remote controller
start up*/
void AbstractController::initClock()
{
    mutex = PTHREAD_MUTEX_INITIALIZER;
    waynesConstant = 1;

    pthread_create(&timer, NULL, runClock, this);
}

/*provides the period for the timer, uses the sleep function to avoid busy
waiting. this is run in a seperate thread and will increment waynesConstant
when each second passes*/
void* runClock(void* ptr)
{
    /*pointer to the controller is required in order to access the constant*/
    AbstractController* controller = (AbstractController*) ptr;

    /*loop forever*/
    while (1)
    {
        /*idle for 1 second*/
        sleep(1);

        while (controller->getWaynesConstant());
        /*std::cerr << "producerdown...";*/
        /*pthread_mutex_lock(controller->getMutex());*/

        /*std::cerr << "producerup!";*/
        /*pthread_mutex_unlock(controller->getMutex());*/
        controller->setWaynesConstant(1);
    }

    return 0;
}

/*accessor*/
pthread_mutex_t* AbstractController::getMutex()
{
    return &mutex;
}

/*accessor*/
int AbstractController::getWaynesConstant()
{
    return waynesConstant;
}

/*used by the seperate thread to set the constant as the thread function is
not a member of the class*/
void AbstractController::setWaynesConstant(int n)
{
    waynesConstant = n;
}

