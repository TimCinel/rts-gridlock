#ifndef ABSTRACTCONTROLLER
#define ABSTRACTCONTROLLER

/*bitwise states - deprecated*/
/*all lights*/
/*#define STARTUP 010101010
#define STARTUP_F 010001010
#define NS_CLEAR 000100001
#define NS_CLEAR_F 000000000
#define NS_STRAIGHT 000100001
#define NS_STRAIGHT_F 000000000
#define EW_CLEAR 000100001
#define EW_CLEAR_F 000000000
#define EW_STRAIGHT 000100001
#define EW_STRAIGHT_F 000000000
#define OTHER_FLASH 000000000*/

/*north-south lights*/
/*#define NS_TRAM_G_NS 100100001
#define NS_TRAM_G_F_NS 000000000
#define NS_TRAM_F_NS 010100001
#define NS_TRAM_F_F_NS 000000000
#define NS_STRAIGHT_G_PED_G_NS 001000100
#define NS_STRAIGHT_G_PED_G_F_NS 000000000
#define NS_STRAIGHT_G_PED_F_NS 000100100
#define NS_STRAIGHT_G_PED_F_F_NS 000100000
#define NS_STRAIGHT_G_NS 000100100
#define NS_STRAIGHT_G_F_NS 000000000
#define NS_STRAIGHT_F_NS 000100010
#define NS_STRAIGHT_F_F_NS 000000000
#define EW_STRAIGHT_G_PED_G_NS 000100001
#define EW_STRAIGHT_G_PED_G_F_NS 000000000
#define EW_STRAIGHT_G_PED_F_NS 000100001
#define EW_STRAIGHT_G_PED_F_F_NS 000000000
#define EW_BOTH_RIGHT_G_NS 000100001
#define EW_BOTH_RIGHT_G_F_NS 000000000
#define EW_BOTH_RIGHT_F_NS 000100001
#define EW_BOTH_RIGHT_F_F_NS 000000000
#define EW_STRAIGHT_G_NS 000100001
#define EW_STRAIGHT_G_F_NS 000000000
#define EW_STRAIGHT_F_NS 000100001
#define EW_STRAIGHT_F_F_NS 000000000*/

/*east-west lights*/
/*#define NS_TRAM_G_EW 000100001
#define NS_TRAM_G_F_EW 000000000
#define NS_TRAM_F_EW 000100001
#define NS_TRAM_F_F_EW 000000000
#define NS_STRAIGHT_G_PED_G_EW 000100001
#define NS_STRAIGHT_G_PED_G_F_EW 000000000
#define NS_STRAIGHT_G_PED_F_EW 000100001
#define NS_STRAIGHT_G_PED_F_F_EW 000000000
#define NS_STRAIGHT_G_EW 000100001
#define NS_STRAIGHT_G_F_EW 000000000
#define NS_STRAIGHT_F_EW 000100001
#define NS_STRAIGHT_F_F_EW 000000000
#define EW_STRAIGHT_G_PED_G_EW 001000100
#define EW_STRAIGHT_G_PED_G_F_EW 000000000
#define EW_STRAIGHT_G_PED_F_EW 000100100
#define EW_STRAIGHT_G_PED_F_F_EW 000100000
#define EW_BOTH_RIGHT_G_EW 000110000
#define EW_BOTH_RIGHT_G_F_EW 000000000
#define EW_BOTH_RIGHT_F_EW 000101000
#define EW_BOTH_RIGHT_F_F_EW 000000000
#define EW_STRAIGHT_G_EW 000100100
#define EW_STRAIGHT_G_F_EW 000000000
#define EW_STRAIGHT_F_EW 000100010
#define EW_STRAIGHT_F_F_EW 000000000*/

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
    }
}

/*****/

#include "Queue.h"

#define INIT_T 0

#define SET 0
#define RESET 1

class AbstractController
{
   /*constructor*/
   public:
      

   /*functions*/
   public:
      virtual void trigger() = 0;
      void init(char*);
      State nextState();
      State getState();
      void tick();
      void setFlag(int);

   /*instance variables*/
   private:
      State s;
      unsigned int t;
      Queue* queue;
};

#endif
