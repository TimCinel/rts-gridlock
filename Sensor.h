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
    char                trigger;
    Controller          *target;
    int                 flagPosition; 

    pthread_t           *thread;
    pthread_attr_t      *threadAttributes;


public:
    Sensor();

    //use this constructor
    Sensor(Controller *target, char trigger, int flagPosition);

    ~Sensor();

    void checkTrigger(char received);

};


namespace sensor_util {
    void *listen(void *args);
}

#endif
