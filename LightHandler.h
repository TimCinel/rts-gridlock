#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H

namespace Light {

    typedef unsigned int lightString; 

    //all light options
    typedef enum lightOption {
        CAR_STRAIGHT_STOP,
        CAR_STRAIGHT_FINISH,
        CAR_STRAIGHT_GO,
        CAR_RIGHT_FINISH,
        CAR_RIGHT_GO,
        PEDESTRIAN_STOP,
        PEDESTRIAN_GO,
        TRAM_FINISH,
        TRAM_GO,
        LIGHTOPTION_SENTINEL
    };

    //conveniently-grouped light options
    static const lightString CAR_STRAIGHT   = (1 << CAR_STRAIGHT_GO) | 
                                              (1 << CAR_STRAIGHT_FINISH) | 
                                              (1 << CAR_STRAIGHT_STOP);

    static const lightString CAR_RIGHT      = (1 << CAR_RIGHT_GO) | 
                                              (1 << CAR_RIGHT_FINISH);

    static const lightString TRAM_STRAIGHT  = (1 << TRAM_GO) | 
                                              (1 << TRAM_FINISH);

    static const lightString PEDESTRIAN     = (1 << PEDESTRIAN_GO) | 
                                              (1 << PEDESTRIAN_STOP);

    //for convenience (using inline instead of macro)
    inline unsigned int bit(unsigned int x) { return (1 << (x)); }

}

using namespace Light;

class LightHandler {
private:

    lightString lightConfiguration;

    lightString lightFlags;
    lightString flashFlags;

public:

    LightHandler();
    LightHandler(lightString lightConfiguration);

    void setState(lightString lights, lightString flashes);

    lightString getConfiguration();
    lightString getLights();
    lightString getFlashes();

    void printState();
};

#endif
