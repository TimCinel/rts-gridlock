#ifndef SENSOR_H
#define SENSOR_H

#include <pthread.h>

//mock Controller class
class Controller {
private:
    int sensorFlags;
public:
    Controller() { sensorFlags = 0; }

    void setSensorFlag(int flagPosition) {

        sensorFlags |= 1 << flagPosition;

        std::cout << "\nFlag set: " << flagPosition << "\n";
    };
};

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
