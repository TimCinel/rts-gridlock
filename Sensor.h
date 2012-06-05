#ifndef SENSOR_H
#define SENSOR_H

/*for testing*/
#ifndef MOCK_OBJECTS
    #include "AbstractController.h"
#else
    #include "test/mock_Controller.h"
#endif

#include <pthread.h>

class Sensor
{
    /*instance variables*/
    private:
        AbstractController  *target; //controller to set flag on
        char trigger; //character to detect when input is read
        int flagPosition; //flag to set when trigger is detected
        int readFD; //file descriptor to read on
        int writeFD; //file descriptor to write on

        pthread_t *thread;    
        pthread_attr_t *threadAttr;

    /*constructors*/
    public:
        Sensor(AbstractController *target, char trigger, int flagPosition,
            int readFD);
        Sensor(AbstractController *target, char trigger, int flagPosition);
        ~Sensor();

    /*member functions*/
    public:
        int getReadFD();
        int getWriteFD();
        int getTrigger();

        void checkTrigger(char received);

    private:
        void createThread();
};

/*non member function for thread*/
namespace sensor_util {
    void *listen(void *args);
}

#endif
