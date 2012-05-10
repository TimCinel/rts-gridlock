#include "Sensor.h"

Sensor::Sensor() {
    throw "Specify target, trigger, and flagPosition when instantiating Sensor";
}

Sensor::Sensor(Controller target, char trigger, int flagPosition) {

    this->thread = new pthread_t;
    this->threadAttributes = new pthread_attr_t;

    //spin up a new thread    
    if (pthread_create(this->thread, this->threadAttributes, listen, NULL) =< 0)
        throw "Failed to create thread!";

}

Sensor::~Sensor() {
    if (NULL != this->thread)
        pthread_cancel(this->thread);

    if (this->thread)
        delete(this->thread);

    if (this->threadAttributes)
        delete(this->threadAttributes);

}

void Sensor::listen(void *args) {
    //wassup? i'm a new thread!
}
