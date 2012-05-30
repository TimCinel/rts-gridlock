#include "IntersectionController.h"

#include <stdio.h>
#include <iostream>

using namespace ControllerInfo;

IntersectionController::IntersectionController()
{
    std::cout << "Constructor\n";

    this->initialiseStates();

}

void IntersectionController::trigger()
{
    if (this->getTime() > 0)
        return;

    (this->*stateRecord[this->state])();
}

void IntersectionController::setFlag(unsigned int flag)
{
    if (flag < CONTROLLER_FLAG_SENTINEL)
        //regular case - just set the flag
        this->flags[flag % CONTROLLER_FLAG_SENTINEL] = 1;
    else
        //special case for modes - only keep one flag with multiple values
        this->flags[SYSTEM_MODE] = flag;
}

int IntersectionController::getFlag(unsigned int flag) 
{
    if (flag < CONTROLLER_FLAG_SENTINEL)
        return this->flags[flag % CONTROLLER_FLAG_SENTINEL];
    else
        return flag == this->flags[SYSTEM_MODE];
}


void IntersectionController::startup()
{
    this->transitionToState(NS_CLEAR, T_NS_CLEAR);
}

void IntersectionController::ns_clear()
{
    if (
            //NS_TRAM_G guards
            this->getFlag(TIMER_MODE) && this->getFlag(SEQ_TRAM) ||
            this->getFlag(COMMAND_MODE) && this->getFlag(CMD_TRAM) ||
            this->getFlag(SENSOR_MODE) && this->getFlag(SEN_TRAM)
       )

        this->transitionToState(NS_TRAM_G, T_TRAM_G);

    else if (
            //NS_STRAIGHT guards
            this->getFlag(SENSOR_MODE) ||
            this->getFlag(COMMAND_MODE) && this->getFlag(CMD_NS_PED) ||
            this->getFlag(COMMAND_MODE) && this->getFlag(CMD_NS_STRAIGHT) ||
            this->getFlag(TIMER_MODE) && this->getFlag(SEQ_NS_STRAIGHT) ||
            this->getFlag(TIMER_MODE) && this->getFlag(SEQ_NS_PED)
            )

        this->transitionToState(NS_STRAIGHT, 0);

    else if (
            //EW_CLEAR guards
            this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_STRAIGHT) || 
            this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_PED) || 
            this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_RIGHT) 
            )

        this->transitionToState(EW_CLEAR, T_EW_CLEAR);

    else
        //no transition defined - crash
        this->transitionToState(STARTUP, T_STARTUP);

}

void IntersectionController::ns_tram_g()
{
    this->transitionToState(NS_TRAM_F, T_TRAM_F);

}

void IntersectionController::ns_tram_f()
{
}

void IntersectionController::ns_straight()
{
}

void IntersectionController::ns_straight_g_ped_g()
{
}

void IntersectionController::ns_straight_g_ped_f()
{
}

void IntersectionController::ns_straight_g()
{
}

void IntersectionController::ns_straight_f()
{
}

void IntersectionController::ew_clear()
{
}

void IntersectionController::ew_both_right_g()
{
}

void IntersectionController::ew_both_right_f()
{
}

void IntersectionController::ew_straight()
{
}

void IntersectionController::ew_straight_g_ped_g()
{
}

void IntersectionController::ew_straight_g_ped_f()
{
}

void IntersectionController::ew_straight_g()
{
}

void IntersectionController::ew_straight_f()
{
}


void IntersectionController::
mapState(controllerState state,
         void (IntersectionController::*stateFunction)(), 
         lightString lightFlagsNS, lightString lightFlagsEW,
         lightString flashFlagsNS, lightString flashFlagsEW
        )
{
    this->stateRecord[state] = stateFunction;
    
    this->lightFlagsNS[state] = lightFlagsNS;
    this->lightFlagsEW[state] = lightFlagsEW;

    this->flashFlagsNS[state] = flashFlagsNS;
    this->flashFlagsEW[state] = flashFlagsEW;
}

void IntersectionController::display()
{
   //vars
   /*
   printf("        x x x        \n");
   printf("        - - -        \n");
   printf("       |G|W|G|       \n");
   printf("       |A|A|R|       \n");
   printf("       |R|- -        \n");
   printf("        -            \n");
   printf("  --             --  \n");
   printf("x|GR|           |RG|x\n");
   printf("  --             --  \n");
   printf("x|WA|           |AW|x\n");
   printf("  ---           ---  \n");
   printf("x|GAR|         |RAG|x\n");
   printf("  ---           ---  \n");
   printf("            -        \n");
   printf("        - -|R|       \n");
   printf("       |R|A|A|       \n");
   printf("       |G|W|G|       \n");
   printf("        - - -        \n");
   printf("        x x x        \n");
   */
}


void IntersectionController::transitionToState(controllerState state, int time)
{
    //TODO: Send message to controller

    //set lights
    for (unsigned int i = 0; i < this->lightsNS.size(); i++)
        this->lightsNS[i]->setState(this->lightFlagsNS[state], 
                                    this->flashFlagsNS[state]);

    for (unsigned int i = 0; i < this->lightsEW.size(); i++)
        this->lightsEW[i]->setState(this->lightFlagsEW[state],
                                    this->flashFlagsEW[state]);

    this->resetTimer(time);
    this->state = state;

    std::cout << "Transitioning to state: " << controllerStateNames[state] << "\n";
    std::cout << "Time: " << time << "\n";

}

void IntersectionController::initialiseStates() 
{
    std::cout << "initialiseStates\n";

    //populate state map
    mapState(STARTUP, &IntersectionController::startup, 
             STARTUP_L_NS, STARTUP_L_EW, 
             STARTUP_F_NS, STARTUP_F_EW);

    mapState(NS_CLEAR, &IntersectionController::ns_clear, 
             NS_CLEAR_L_NS, NS_CLEAR_L_EW, 
             NS_CLEAR_F_NS, NS_CLEAR_F_EW);

    mapState(NS_TRAM_G, &IntersectionController::ns_tram_g,
             NS_TRAM_G_L_NS, NS_TRAM_G_L_EW,
             NS_TRAM_G_F_NS, NS_TRAM_G_F_EW);

    mapState(NS_TRAM_F, &IntersectionController::ns_tram_f,
             NS_TRAM_F_L_NS, NS_TRAM_F_L_EW,
             NS_TRAM_F_F_NS, NS_TRAM_F_F_EW);

    mapState(NS_STRAIGHT, &IntersectionController::ns_straight,
             NS_STRAIGHT_L_NS, NS_STRAIGHT_L_EW,
             NS_STRAIGHT_F_NS, NS_STRAIGHT_F_EW);

    mapState(NS_STRAIGHT_G_PED_G, &IntersectionController::ns_straight_g_ped_g,
             NS_STRAIGHT_G_PED_G_L_NS, NS_STRAIGHT_G_PED_G_L_EW,
             NS_STRAIGHT_G_PED_G_F_NS, NS_STRAIGHT_G_PED_G_F_EW);

    mapState(NS_STRAIGHT_G_PED_F, &IntersectionController::ns_straight_g_ped_f,
             NS_STRAIGHT_G_PED_F_L_NS, NS_STRAIGHT_G_PED_F_L_EW,
             NS_STRAIGHT_G_PED_F_F_NS, NS_STRAIGHT_G_PED_F_F_EW);

    mapState(NS_STRAIGHT_G_PED_F, &IntersectionController::ns_straight_g_ped_f,
             NS_STRAIGHT_G_PED_F_L_NS, NS_STRAIGHT_G_PED_F_L_EW,
             NS_STRAIGHT_G_PED_F_F_NS, NS_STRAIGHT_G_PED_F_F_EW);

    mapState(NS_STRAIGHT_G, &IntersectionController::ns_straight_g,
             NS_STRAIGHT_G_L_NS, NS_STRAIGHT_G_L_EW,
             NS_STRAIGHT_G_F_NS, NS_STRAIGHT_G_F_EW);

    mapState(NS_STRAIGHT_F, &IntersectionController::ns_straight_f,
             NS_STRAIGHT_F_L_NS, NS_STRAIGHT_F_L_EW,
             NS_STRAIGHT_F_F_NS, NS_STRAIGHT_F_F_EW);

    mapState(EW_CLEAR, &IntersectionController::ew_clear,
             EW_CLEAR_L_NS, EW_CLEAR_L_EW,
             EW_CLEAR_F_NS, EW_CLEAR_F_EW);

    mapState(EW_BOTH_RIGHT_G, &IntersectionController::ew_both_right_g,
             EW_BOTH_RIGHT_G_L_NS, EW_BOTH_RIGHT_G_L_EW,
             EW_BOTH_RIGHT_G_F_NS, EW_BOTH_RIGHT_G_F_EW);

    mapState(EW_BOTH_RIGHT_F, &IntersectionController::ew_both_right_f,
             EW_BOTH_RIGHT_F_L_NS, EW_BOTH_RIGHT_F_L_EW,
             EW_BOTH_RIGHT_F_F_NS, EW_BOTH_RIGHT_F_F_EW);

    mapState(EW_STRAIGHT, &IntersectionController::ew_straight,
             EW_STRAIGHT_L_NS, EW_STRAIGHT_L_EW,
             EW_STRAIGHT_F_NS, EW_STRAIGHT_F_EW);

    mapState(EW_STRAIGHT_G_PED_G, &IntersectionController::ew_straight_g_ped_g,
             EW_STRAIGHT_G_PED_G_L_NS, EW_STRAIGHT_G_PED_G_L_EW,
             EW_STRAIGHT_G_PED_G_F_NS, EW_STRAIGHT_G_PED_G_F_EW);

    mapState(EW_STRAIGHT_G_PED_F, &IntersectionController::ew_straight_g_ped_f,
             EW_STRAIGHT_G_PED_F_L_NS, EW_STRAIGHT_G_PED_F_L_EW,
             EW_STRAIGHT_G_PED_F_F_NS, EW_STRAIGHT_G_PED_F_F_EW);

    mapState(EW_STRAIGHT_G, &IntersectionController::ew_straight_g,
             EW_STRAIGHT_G_L_NS, EW_STRAIGHT_G_L_EW,
             EW_STRAIGHT_G_F_NS, EW_STRAIGHT_G_F_EW);

    mapState(EW_STRAIGHT_F, &IntersectionController::ew_straight_f,
             EW_STRAIGHT_F_L_NS, EW_STRAIGHT_F_L_EW,
             EW_STRAIGHT_F_F_NS, EW_STRAIGHT_F_F_EW);


    //add some lights
    this->lightsNS.push_back(new LightHandler(I1_I3_NS_LIGHT_CONF));
    this->lightsNS.push_back(new LightHandler(I1_I3_NS_LIGHT_CONF));

    this->lightsNS.push_back(new LightHandler(I1_I3_NS_LIGHT_CONF));
    this->lightsNS.push_back(new LightHandler(I1_I3_NS_LIGHT_CONF));


    //default to sequence mode
    this->setFlag(TIMER_MODE);

    //default sequence
    this->setFlag(SEQ_NS_STRAIGHT);
    this->setFlag(SEQ_NS_PED);

    this->setFlag(SEQ_EW_RIGHT);
    this->setFlag(SEQ_EW_STRAIGHT);
    this->setFlag(SEQ_EW_PED);

    //initial state
    this->transitionToState(STARTUP, T_STARTUP);

}
