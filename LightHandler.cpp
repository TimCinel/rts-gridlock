#include "LightHandler.h"
#include <stdio.h>

LightHandler::LightHandler() {
    throw "Specify lightConfiguration when instantiating LightHandler";
}

LightHandler::LightHandler(int lightConfiguration) : 
    lightConfiguration(lightConfiguration) {
}

void LightHandler::setState(int lights, int flashes) {
    //only enable lights that exist in this configuration
    this->lightFlags = this->lightConfiguration & lights;
    this->flashFlags = this->lightConfiguration & flashes;
}

int LightHandler::getConfiguration() {
    return this->lightConfiguration;
}

int LightHandler::getLights() {
    return this->lightFlags;
}

int LightHandler::getFlashes() {
    return this->flashFlags;
}

void LightHandler::printState() {
    for (int i = 0; i < 3; i++) {

        char *message, *positive, *negative;
        int subject;

        switch (i) {
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

        printf("%s (%d)\n", message, subject);

        int j = 0;
        do {
            printf("\t%s: %s\n", 
                    light::lightOptionNames[j], 
                    (1 << j & subject ? positive : negative )
                  );
        } while (++j < light::SENTINEL);

    }
}
