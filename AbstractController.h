#ifndef ABSTRACTCONTROLLER
#define ABSTRACTCONTROLLER

#include "AbstractController.h"

#include <pthread.h>
#include <iostream>

#define SUCCESS 1
#define FAILURE 0

class AbstractController
{
    /*functions*/
    public:
        /*purely virtual, implemented by subclasses*/
        virtual void trigger() = 0;
        virtual void receiveMessage(char *sender, int header, int msg) = 0;
        virtual void clearFlag(unsigned int flag) = 0;
        virtual void setFlag(unsigned int flag) = 0;
        virtual int getFlag(unsigned int flag) = 0;
        virtual void display() = 0;

        void tick();
        void resetTimer(int time);
        int getTime();

        void initClock();

        int getWaynesConstant();
        void setWaynesConstant(int);

        pthread_mutex_t* getMutex();

    /*instance variables*/
    private:
        int s;
        int time;
        pthread_t timer;
        pthread_mutex_t mutex;
        int waynesConstant;
};

/*run in a seperate thread, cannot be a class member*/
void* runClock(void*);

#endif
