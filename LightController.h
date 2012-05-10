#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

//namespace?
enum lightOption {
    LIGHT_CAR_S_R,
    LIGHT_CAR_S_A,
    LIGHT_CAR_S_G,
    LIGHT_CAR_R_R,
    LIGHT_CAR_R_A,
    LIGHT_CAR_R_G,
    LIGHT_PED_R,
    LIGHT_PED_G,
    LIGHT_TRAM_R,
    LIGHT_TRAM_A,
    LIGHT_TRAM_G,
    LIGHT_ELEMENTS
}

class LightController {
private:
    int lightConfiguration;
public:
    LightController();

    LightController(int lightConfiguration) { lightConfiguration = lightConfiguration; }
};

#endif
