#include "LightHandler.h"
#include <iostream>

namespace Light
{
    /*mainly for debug purposes, contains the different key states*/
    static const char *lightOptionNames[] = 
    {
        "CAR_STRAIGHT_STOP",
        "CAR_STRAIGHT_FINISH",
        "CAR_STRAIGHT_GO",
        "CAR_RIGHT_FINISH",
        "CAR_RIGHT_GO",
        "PEDESTRIAN_STOP",
        "PEDESTRIAN_GO",
        "TRAM_FINISH",
        "TRAM_GO"
    };
}

using namespace Light;

/*constructor*/
LightHandler::LightHandler(lightString lightConfiguration) : 
    lightConfiguration(lightConfiguration) 
{

}

/*set the lights value of a state*/
void LightHandler::setState(lightString lights, lightString flashes) 
{
    /*only enable lights that exist in this configuration*/
    this->lightFlags = this->lightConfiguration & lights;
    this->flashFlags = this->lightConfiguration & flashes;
}

/*accessor*/
lightString LightHandler::getConfiguration() 
{
    return this->lightConfiguration;
}

/*accessor*/
lightString LightHandler::getLights()
{
    return this->lightFlags;
}

/*accesor*/
lightString LightHandler::getFlashes() 
{
    return this->flashFlags;
}

/*displays the state of a light, used for debugging only*/
void LightHandler::printState()
{
    for (int i = 0; i < 3; i++)
    {

        char *message, *positive, *negative;
        lightString subject;

        switch (i)
        {
            case 0:
                subject = this->lightConfiguration;
                message = (char *)"= Light Configuration =";
                positive = (char *)"Enabled";
                negative = (char *)"Disabled";
                break;
            case 1:
                subject = this->lightFlags;
                message = (char *)"= Light State =";
                positive = (char *)"On";
                negative = (char *)"Off";
                break;
            case 2:
                subject = this->flashFlags;
                message = (char *)"= Flash State =";
                positive = (char *)"On";
                negative = (char *)"Off";
                break;
        }

        /*string representation of lights*/
        printf("%s (%d)\n", message, subject);

        /*bit string representation of lights*/
        int j = 0;
        do
        {
            printf("\t%s: %s\n", 
                    lightOptionNames[j], 
                    (1 << j & subject ? positive : negative )
                  );
        } while (++j < LIGHTOPTION_SENTINEL);
    }
}

