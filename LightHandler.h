#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H

namespace light {
    enum lightOption {
        CAR_STRAIGHT_STOP,
        CAR_STRAIGHT_FINISH,
        CAR_STRAIGHT_GO,
        CAR_RIGHT_FINISH,
        CAR_RIGHT_GO,
        PEDESTRIAN_STOP,
        PEDESTRIAN_GO,
        TRAM_FINISH,
        TRAM_GO,
        SENTINEL
    };

    const char *lightOptionNames[] = {
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

class LightHandler {
private:

    int lightConfiguration;

    int lightFlags;
    int flashFlags;

public:
    static const int CAR_STRAIGHT = 1 << light::CAR_STRAIGHT_GO | 
                                    1 << light::CAR_STRAIGHT_FINISH | 
                                    1 << light::CAR_STRAIGHT_STOP;

    static const int PEDESTRIAN =   1 << light::PEDESTRIAN_GO | 
                                    1 << light::PEDESTRIAN_STOP;

    LightHandler();
    LightHandler(int lightConfiguration);

    void setState(int lights, int flashes);

    int getConfiguration();
    int getLights();
    int getFlashes();

    void printState();
};

#endif
