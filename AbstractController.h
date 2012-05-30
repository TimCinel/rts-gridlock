#ifndef ABSTRACTCONTROLLER
#define ABSTRACTCONTROLLER

#include "AbstractController.h"
#include <pthread.h>

#define SUCCESS 1
#define FAILURE 0

void* runClock(void*);

class AbstractController
{
public:
    /*functions*/
    virtual void trigger() = 0;
    void tick();

    inline void resetTimer(int time) { this->time = time; }
    inline int getTime() { return this->time; }

    virtual void clearFlag(unsigned int flag) = 0;
    virtual void setFlag(unsigned int flag) = 0;
    virtual int getFlag(unsigned int flag) = 0;

    void initClock();
    void endClock();
    void downMutex();
    void upMutex();

    int getWaynesConstant();
    void setWaynesConstant();
    void clearWaynesConstant();
    void toggleWaynesConstant();

private:
    /*instance variables*/
    int s;
    int time;
    pthread_t timer;
    pthread_mutex_t mutex;
    int waynesConstant;
};

#endif
