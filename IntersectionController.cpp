#include "IntersectionController.h"

#include <stdio.h>

using namespace ControllerStates;

IntersectionController::IntersectionController() {
}

void IntersectionController::initialiseStates() 
{
    mapState(STARTUP, &IntersectionController::startup, 
             STARTUP_L_NS, STARTUP_L_EW, 
             STARTUP_F_NS, STARTUP_F_EW);

    /*
    map_state_function(NS_TRAM_G, ns_tram_g);
    map_state_function(NS_TRAM_F, ns_tram_f);
    map_state_function(NS_STRAIGHT, ns_straight);
    map_state_function(NS_STRAIGHT_G_PED_G, ns_straight_g_ped_g);
    map_state_function(NS_STRAIGHT_G_PED_F, ns_straight_g_ped_f);
    map_state_function(NS_STRAIGHT_G_PED_F, ns_straight_g_ped_f);
    map_state_function(NS_STRAIGHT_G, ns_straight_g);
    map_state_function(NS_STRAIGHT_F, ns_straight_f);
    map_state_function(EW_CLEAR, ew_clear);
    map_state_function(EW_BOTH_RIGHT_G, ew_both_right_g);
    map_state_function(EW_BOTH_RIGHT_F, ew_both_right_f);
    map_state_function(EW_STRAIGHT, ew_straight);
    map_state_function(EW_STRAIGHT_G_PED_G, ew_straight_g_ped_g);
    map_state_function(EW_STRAIGHT_G_PED_F, ew_straight_g_ped_f);
    map_state_function(EW_STRAIGHT_G, ew_straight_g);
    map_state_function(EW_STRAIGHT_F, ew_straight_f);
    */
}





void IntersectionController::startup() {
}

void IntersectionController::ns_clear() {
}

void IntersectionController::ns_tram_g() {
}

void IntersectionController::ns_tram_f() {
}

void IntersectionController::ns_straight() {
}

void IntersectionController::ns_straight_g_ped_g() {
}

void IntersectionController::ns_straight_g_ped_f() {
}

void IntersectionController::ns_straight_g() {
}

void IntersectionController::ns_straight_f() {
}

void IntersectionController::ew_clear() {
}

void IntersectionController::ew_both_right_g() {
}

void IntersectionController::ew_both_right_f() {
}

void IntersectionController::ew_straight() {
}

void IntersectionController::ew_straight_g_ped_g() {
}

void IntersectionController::ew_straight_g_ped_f() {
}

void IntersectionController::ew_straight_g() {
}

void IntersectionController::ew_straight_f() {
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

void IntersectionController::trigger() {
}

void IntersectionController::display()
{
   //vars
   
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
}



