#ifndef ABSTRACTCONTROLLER
#define ABSTRACTCONTROLLER

#include "AbstractController.h"
#include <pthread.h>

#define SUCCESS 1
#define FAILURE 0

class AbstractController
{
    /*functions*/
    public:
        virtual void trigger() = 0;
        virtual void receiveMessage(char *sender, int header, int msg) = 0;
        virtual void clearFlag(unsigned int flag) = 0;
        virtual void setFlag(unsigned int flag) = 0;
        virtual int getFlag(unsigned int flag) = 0;

        void tick();
        inline void resetTimer(int time);
        inline int getTime();

        void initClock();
        void endClock();

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

void* runClock(void*);

#endif
