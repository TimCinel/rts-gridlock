#include <iostream>
#ifndef CONTROLLER_H
#define CONTROLLER_H

class AbstractController {
    /*instance variables*/
    private:
        int sensorFlags;

    /*constructor*/
    public:
        AbstractController()
        {
            sensorFlags = 0;
        }

    /*member functions - defined inline as class is only for testing*/
    public:
        void setFlag(int flagPosition)
        {
            sensorFlags |= 1 << flagPosition;
            std::cout << "\nFlag set: " << flagPosition << "\n";
        };
};

#endif

