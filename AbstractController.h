#ifndef ABSTRACTCONTROLLER
#define ABSTRACTCONTROLLER

namespace ControllerStates {
/*state format: NSCar|NSTram|NSPed|EWCar|EWCarTurning|EWPed*/
/*state values: R - Red, A - Amber, G - Green, F - Flashing (Amber for Car, Red for Ped), C - Cleared (not on), W - White (tram only)*/
    typedef enum controllerState {
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
};

namespace ControllerFlags {
    typedef enum {
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

class AbstractController
{
   /*constructor*/
   public:
      

   /*functions*/
   public:
      virtual void trigger() = 0;
      controllerState nextState();
      controllerState getState();
      void tick();

   /*instance variables*/
   private:
      controllerState s;
      unsigned int t;
};

#endif
