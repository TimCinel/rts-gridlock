#ifndef SENSOR_H
#define SENSOR_H

class Sensor {

private:
    char                trigger;
    Controller          target;
    int                 flagPosition; 

    pthread_t           *thread;
    pthread_attr_t      *threadAttributes;


public:
    Sensor();

    //use this constructor
    Sensor(Controller target, char trigger, int flagPosition) 
        {target = target, trigger = trigger, flagPosition = flagPosition,
         thread = NULL, threadAttributes = NULL; }

    ~Sensor();

private:
    //private method - listens for simulated sensor input in another thread
    void listen(void *args);

};

#endif
