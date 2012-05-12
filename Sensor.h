#ifndef SENSOR_H
#define SENSOR_H

#ifndef MOCK_OBJECTS
#include "Controller.h"
#else
#include "test/mock_Controller.h"
#endif

#include <pthread.h>


class Sensor {

private:
    char                trigger;        //character to detect when input is read
    Controller          *target;        //controller to set flag on
    int                 flagPosition;   //flag to set when trigger is detected
    int                 readFd;         //file descriptor to read on

    pthread_t           *thread;    
    pthread_attr_t      *threadAttr;


public:
    Sensor();

    //use this constructor
    Sensor(Controller *target, char trigger, int flagPosition, int readFd);
    ~Sensor();

    int getReadFd() {return this->readFd;}
    int getTrigger() {return this->trigger;}

    void checkTrigger(char received);

};


namespace sensor_util {
    void *listen(void *args);
}

#endif
