#include <iostream>
#ifndef CONTROLLER_H
#define CONTROLLER_H

class AbstractController {
private:
    int sensorFlags;
public:
    AbstractController() { sensorFlags = 0; }

    void setFlag(int flagPosition) {

        sensorFlags |= 1 << flagPosition;

        std::cout << "\nFlag set: " << flagPosition << "\n";
    };
};

#endif
