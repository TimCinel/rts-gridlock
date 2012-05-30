#ifndef INTERSECTIONCONTROLLER
#define INTERSECTIONCONTROLLER

#include "AbstractController.h"
#include "LightHandler.h"

#include <vector>


namespace ControllerInfo
{

    //STATE CONSTANTS

    typedef enum
    {
        STARTUP,
        NS_CLEAR,
        NS_TRAM_G,
        NS_TRAM_F,
        NS_STRAIGHT,
        NS_STRAIGHT_G_PED_G,
        NS_STRAIGHT_G_PED_F,
        NS_STRAIGHT_G,
        NS_STRAIGHT_F,
        EW_CLEAR,
        EW_BOTH_RIGHT_G,
        EW_BOTH_RIGHT_F,
        EW_STRAIGHT,
        EW_STRAIGHT_G_PED_G,
        EW_STRAIGHT_G_PED_F,
        EW_STRAIGHT_G,
        EW_STRAIGHT_F,
        CONTROLLER_STATE_SENTINAL
    } controllerState;

    static const char *controllerStateNames[] = {
        "STARTUP",
        "NS_CLEAR",
        "NS_TRAM_G",
        "NS_TRAM_F",
        "NS_STRAIGHT",
        "NS_STRAIGHT_G_PED_G",
        "NS_STRAIGHT_G_PED_F",
        "NS_STRAIGHT_G",
        "NS_STRAIGHT_F",
        "EW_CLEAR",
        "EW_BOTH_RIGHT_G",
        "EW_BOTH_RIGHT_F",
        "EW_STRAIGHT",
        "EW_STRAIGHT_G_PED_G",
        "EW_STRAIGHT_G_PED_F",
        "EW_STRAIGHT_G",
        "EW_STRAIGHT_F",
        "CONTROLLER_STATE_SENTINAL"
    };


    //CONTROLLER CONSTANTS

    typedef enum 
    {
        SYSTEM_MODE,
        CMD_EW_STRAIGHT,
        CMD_EW_PED,
        CMD_EW_RIGHT,
        CMD_NS_STRAIGHT,
        CMD_NS_PED, 
        CMD_TRAM, 
        SEN_EW_STRAIGHT,
        SEN_EW_PED,
        SEN_EW_RIGHT,
        SEN_NS_STRAIGHT,
        SEN_NS_PED, 
        SEN_TRAM, 
        SEQ_EW_STRAIGHT,
        SEQ_EW_PED,
        SEQ_EW_RIGHT,
        SEQ_NS_STRAIGHT,
        SEQ_NS_PED, 
        SEQ_TRAM, 
        CONTROLLER_FLAG_SENTINEL,
        COMMAND_MODE,
        SENSOR_MODE,
        TIMER_MODE
    } controllerFlag;

    static const char *controllerFlagNames[] = {
        "SYSTEM_MODE",
        "CMD_EW_STRAIGHT",
        "CMD_EW_PED",
        "CMD_EW_RIGHT",
        "CMD_NS_STRAIGHT",
        "CMD_NS_PED",
        "CMD_TRAM",
        "SEN_EW_STRAIGHT",
        "SEN_EW_PED",
        "SEN_EW_RIGHT",
        "SEN_NS_STRAIGHT",
        "SEN_NS_PED", 
        "SEN_TRAM", 
        "SEQ_EW_STRAIGHT",
        "SEQ_EW_PED",
        "SEQ_EW_RIGHT",
        "SEQ_NS_STRAIGHT",
        "SEQ_NS_PED", 
        "SEQ_TRAM", 
        "CONTROLLER_FLAG_SENTINEL",
        "COMMAND_MODE",
        "SENSOR_MODE",
        "TIMER_MODE"
    };


    //TIME CONSTANTS

    static const unsigned int T_STARTUP             = 15;

    static const unsigned int T_NS_CLEAR            = 2;
    static const unsigned int T_NS_STRAIGHT_G       = 20;
    static const unsigned int T_NS_STRAIGHT_CHECK   = 5;
    static const unsigned int T_NS_STRAIGHT_G_QUICK = 5;
    static const unsigned int T_NS_STRAIGHT_F       = 4;
    static const unsigned int T_NS_PED_G            = 20;
    static const unsigned int T_NS_PED_F            = 15;

    static const unsigned int T_EW_CLEAR            = 2;
    static const unsigned int T_EW_BOTH_RIGHT_G     = 10;
    static const unsigned int T_EW_BOTH_RIGHT_F     = 4;
    static const unsigned int T_EW_STRAIGHT_G       = 20;
    static const unsigned int T_EW_STRAIGHT_CHECK   = 5;
    static const unsigned int T_EW_STRAIGHT_G_QUICK = 5;
    static const unsigned int T_EW_STRAIGHT_F       = 20;
    static const unsigned int T_EW_PED_G            = 20;
    static const unsigned int T_EW_PED_F            = 15;


    //LIGHT CONSTANTS

    using namespace Light;

    //light configurations

    static const lightString I1_I3_NS_LIGHT_CONF =  CAR_STRAIGHT | 
                                                    PEDESTRIAN;
    static const lightString I1_I3_EW_LIGHT_CONF =  CAR_STRAIGHT |
                                                    PEDESTRIAN |
                                                    CAR_RIGHT;

    static const lightString I2_NS_LIGHT_CONF    =  CAR_STRAIGHT |
                                                    PEDESTRIAN |
                                                    TRAM_STRAIGHT;
    static const lightString I2_EW_LIGHT_CONF    =  CAR_STRAIGHT |
                                                    PEDESTRIAN |
                                                    CAR_RIGHT;

    //light patterns for general traffic

    static const lightString STARTUP_L_NS =         (1 << CAR_STRAIGHT_FINISH) | 
                                                    (1 << PEDESTRIAN_STOP);
    static const lightString STARTUP_L_EW =         (1 << CAR_STRAIGHT_FINISH) | 
                                                    (1 << PEDESTRIAN_STOP);
    static const lightString STARTUP_F_NS =         (1 << CAR_STRAIGHT_FINISH) | 
                                                    (1 << PEDESTRIAN_STOP);
    static const lightString STARTUP_F_EW =         (1 << CAR_STRAIGHT_FINISH) | 
                                                    (1 << PEDESTRIAN_STOP);
    
    //light paterns for north-south 

    static const lightString NS_CLEAR_L_NS =        ALL_STOP;
    static const lightString NS_CLEAR_L_EW =        ALL_STOP;
    static const lightString NS_CLEAR_F_NS =        0;
    static const lightString NS_CLEAR_F_EW =        0;

    static const lightString NS_TRAM_G_L_NS =       (1 << TRAM_GO) | 
                                                    (1 << CAR_STRAIGHT_STOP) | 
                                                    (1 << PEDESTRIAN_STOP);
    static const lightString NS_TRAM_G_L_EW =       ALL_STOP;
    static const lightString NS_TRAM_G_F_NS =       0;
    static const lightString NS_TRAM_G_F_EW =       0;

    static const lightString NS_TRAM_F_L_NS =       (1 << TRAM_FINISH) | 
                                                    (1 << CAR_STRAIGHT_STOP) | 
                                                    (1 << PEDESTRIAN_STOP);
    static const lightString NS_TRAM_F_L_EW =       ALL_STOP;
    static const lightString NS_TRAM_F_F_NS =       0;
    static const lightString NS_TRAM_F_F_EW =       0;

    static const lightString NS_STRAIGHT_L_NS =     ALL_STOP;
    static const lightString NS_STRAIGHT_L_EW =     ALL_STOP;
    static const lightString NS_STRAIGHT_F_NS =     0;
    static const lightString NS_STRAIGHT_F_EW =     0;

    static const lightString NS_STRAIGHT_G_PED_G_L_NS = (1 << CAR_STRAIGHT_GO) |
                                                        (1 << PEDESTRIAN_GO);
    static const lightString NS_STRAIGHT_G_PED_G_L_EW = ALL_STOP;
    static const lightString NS_STRAIGHT_G_PED_G_F_NS = 0;
    static const lightString NS_STRAIGHT_G_PED_G_F_EW = 0;

    static const lightString NS_STRAIGHT_G_PED_F_L_NS = (1 << CAR_STRAIGHT_GO) |
                                                        (1 << PEDESTRIAN_STOP);
    static const lightString NS_STRAIGHT_G_PED_F_L_EW = ALL_STOP;
    static const lightString NS_STRAIGHT_G_PED_F_F_NS = (1 << PEDESTRIAN_STOP);
    static const lightString NS_STRAIGHT_G_PED_F_F_EW = 0;

    static const lightString NS_STRAIGHT_G_L_NS =       (1 << CAR_STRAIGHT_GO) |
                                                        (1 << PEDESTRIAN_STOP);
    static const lightString NS_STRAIGHT_G_L_EW =       ALL_STOP;
    static const lightString NS_STRAIGHT_G_F_NS =       0;
    static const lightString NS_STRAIGHT_G_F_EW =       0;

    static const lightString NS_STRAIGHT_F_L_NS =       (1 << CAR_STRAIGHT_FINISH) |
                                                        (1 << PEDESTRIAN_STOP);
    static const lightString NS_STRAIGHT_F_L_EW =       ALL_STOP;
    static const lightString NS_STRAIGHT_F_F_NS =       0;
    static const lightString NS_STRAIGHT_F_F_EW =       0;

    
    //light patterns for east-west traffic flow
    
    static const lightString EW_CLEAR_L_NS = (1 << CAR_STRAIGHT_FINISH) | 
                                             (1 << PEDESTRIAN_STOP);
    static const lightString EW_CLEAR_L_EW = (1 << CAR_STRAIGHT_FINISH) |
                                             (1 << PEDESTRIAN_STOP);
    static const lightString EW_CLEAR_F_NS = (1 << CAR_STRAIGHT_FINISH) |
                                             (1 << PEDESTRIAN_STOP);
    static const lightString EW_CLEAR_F_EW = (1 << CAR_STRAIGHT_FINISH) |
                                             (1 << PEDESTRIAN_STOP);

    static const lightString EW_BOTH_RIGHT_G_L_NS = ALL_STOP;
    static const lightString EW_BOTH_RIGHT_G_L_EW = (1 << CAR_RIGHT_GO) |
                                                    (1 << CAR_STRAIGHT_STOP) |
                                                    (1 << PEDESTRIAN_STOP);
    static const lightString EW_BOTH_RIGHT_G_F_NS = 0;
    static const lightString EW_BOTH_RIGHT_G_F_EW = 0;

    static const lightString EW_BOTH_RIGHT_F_L_NS = ALL_STOP;
    static const lightString EW_BOTH_RIGHT_F_L_EW = (1 << CAR_RIGHT_FINISH) |
                                                    (1 << CAR_STRAIGHT_STOP) |
                                                    (1 << PEDESTRIAN_STOP);
    static const lightString EW_BOTH_RIGHT_F_F_NS = 0;
    static const lightString EW_BOTH_RIGHT_F_F_EW = 0;

    static const lightString EW_STRAIGHT_L_NS = ALL_STOP;
    static const lightString EW_STRAIGHT_L_EW = ALL_STOP;
    static const lightString EW_STRAIGHT_F_NS = 0;
    static const lightString EW_STRAIGHT_F_EW = 0;

    static const lightString EW_STRAIGHT_G_PED_G_L_NS = ALL_STOP;
    static const lightString EW_STRAIGHT_G_PED_G_L_EW = (1 << CAR_STRAIGHT_GO) |
                                                        (1 << PEDESTRIAN_GO);
    static const lightString EW_STRAIGHT_G_PED_G_F_NS = 0;
    static const lightString EW_STRAIGHT_G_PED_G_F_EW = 0;

    static const lightString EW_STRAIGHT_G_PED_F_L_NS = ALL_STOP;
    static const lightString EW_STRAIGHT_G_PED_F_L_EW = (1 << CAR_STRAIGHT_GO) |
                                                        (1 << PEDESTRIAN_STOP);
    static const lightString EW_STRAIGHT_G_PED_F_F_NS = 0;
    static const lightString EW_STRAIGHT_G_PED_F_F_EW = (1 << PEDESTRIAN_STOP);

    static const lightString EW_STRAIGHT_G_L_NS = ALL_STOP;
    static const lightString EW_STRAIGHT_G_L_EW = (1 << CAR_STRAIGHT_GO) |
                                                  (1 << PEDESTRIAN_STOP);
    static const lightString EW_STRAIGHT_G_F_NS = 0;
    static const lightString EW_STRAIGHT_G_F_EW = 0;

    static const lightString EW_STRAIGHT_F_L_NS = ALL_STOP;
    static const lightString EW_STRAIGHT_F_L_EW = (1 << CAR_STRAIGHT_FINISH) |
                                                  (1 << PEDESTRIAN_STOP);
    static const lightString EW_STRAIGHT_F_F_NS = 0;
    static const lightString EW_STRAIGHT_F_F_EW = 0;

};

class IntersectionController : public AbstractController
{
public:
    //constructor
    IntersectionController();

    //overriding abstract methods
    virtual void trigger();

    virtual void setFlag(unsigned int flag);
    virtual int getFlag(unsigned int flag);

    void display();

private:
    //instance variables
    ControllerInfo::controllerState state;
    unsigned int flags[ControllerInfo::CONTROLLER_FLAG_SENTINEL];

    //a CONTROLLER_STATE_SENTINAL-element array of function pointers
    void (IntersectionController::*stateRecord
            [ControllerInfo::CONTROLLER_STATE_SENTINAL])();

    //storage for light and flash configurations for each state
    std::vector<LightHandler *> lightsNS;
    std::vector<LightHandler *> lightsEW;

    Light::lightString lightFlagsNS[ControllerInfo::CONTROLLER_STATE_SENTINAL];
    Light::lightString lightFlagsEW[ControllerInfo::CONTROLLER_STATE_SENTINAL];

    Light::lightString flashFlagsNS[ControllerInfo::CONTROLLER_STATE_SENTINAL];
    Light::lightString flashFlagsEW[ControllerInfo::CONTROLLER_STATE_SENTINAL];

private:
    virtual void transitionToState(ControllerInfo::controllerState state, int time);
    virtual void initialiseStates();

    void mapState(ControllerInfo::controllerState state,
                  void (IntersectionController::*stateRecord)(), 
                  Light::lightString lightFlagsNS, Light::lightString lightFlagsEW,
                  Light::lightString flashFlagsNS, Light::lightString flashFlagsEW
                 );

    //private state functions
    virtual void startup();
    virtual void ns_clear();
    virtual void ns_tram_g();
    virtual void ns_tram_f();
    virtual void ns_straight();
    virtual void ns_straight_g_ped_g();
    virtual void ns_straight_g_ped_f();
    virtual void ns_straight_g();
    virtual void ns_straight_f();
    virtual void ew_clear();
    virtual void ew_both_right_g();
    virtual void ew_both_right_f();
    virtual void ew_straight();
    virtual void ew_straight_g_ped_g();
    virtual void ew_straight_g_ped_f();
    virtual void ew_straight_g();
    virtual void ew_straight_f();

};

#endif
