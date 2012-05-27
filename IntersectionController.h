#ifndef INTERSECTIONCONTROLLER
#define INTERSECTIONCONTROLLER

#include "AbstractController.h"
#include "LightHandler.h"

using namespace Light;

namespace ControllerStates 
{
    typedef enum controllerState 
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
    };

    const lightString STARTUP_L_NS = (1 << CAR_STRAIGHT_FINISH);
    const lightString STARTUP_L_EW = (1 << CAR_STRAIGHT_FINISH);
    const lightString STARTUP_F_NS = (1 << CAR_STRAIGHT_FINISH);
    const lightString STARTUP_F_EW = (1 << CAR_STRAIGHT_FINISH);

        /*
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
        */

};

namespace ControllerFlags {
    typedef enum controllerFlag {
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
    };
};

using namespace ControllerStates;

class IntersectionController : public AbstractController
{
public:
    //constructor
    IntersectionController();

    //functions
    virtual void trigger();
    void display();

private:
    //instance variables

    //a CONTROLLER_STATE_SENTINAL-element array of function pointers
    void (IntersectionController::*stateRecord[CONTROLLER_STATE_SENTINAL])();

    //storage for light and flash configurations for each state
    lightString lightFlagsNS[CONTROLLER_STATE_SENTINAL];
    lightString lightFlagsEW[CONTROLLER_STATE_SENTINAL];

    lightString flashFlagsNS[CONTROLLER_STATE_SENTINAL];
    lightString flashFlagsEW[CONTROLLER_STATE_SENTINAL];

private:
    virtual void initialiseStates();

    void mapState(controllerState state,
                  void (IntersectionController::*stateRecord)(), 
                  lightString lightFlagsNS, lightString lightFlagsEW,
                  lightString flashFlagsNS, lightString flashFlagsEW
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
