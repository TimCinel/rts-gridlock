#include "AbstractController.h"

#include <iostream>

//State AbstractController::nextState()
//{
//   switch (s)
//   {
//      case (STARTUP)
//         s = NS_CLEAR;
//         break;
//      case (NS_CLEAR)
//      {
//         if (mode == COMMAND)
//            s = EW_CLEAR;
//         else if ()
//            s = NS_TRAM_G;
//         else if ()
//            s = NS STRAIGHT;
//         break;
//      }
//      case (NS_TRAM_G)
//         
//         break;
//      case (NS_TRAM_F)
//         
//         break;
//      case (NS_STRAIGHT)
//         
//         break;
//      case (NS_STRAIGHT_G_PED_G)
//         
//         break;
//      case (NS_STRAIGHT_G_PED_F)
//         
//         break;
//      case (NS_STRAIGHT_G)
//         
//         break;
//      case (NS_STRAIGHT_F)
//         
//         break;
//      case (EW_CLEAR)
//         
//         break;
//      case (EW_BOTH_RIGHT_G)
//         
//         break;
//      case (EW_BOTH_RIGHT_F)
//         
//         break;
//      case (EW_STRAIGHT)
//         
//         break;
//      case (EW_STRAIGHT_G_PED_G)
//         
//         break;
//      case (EW_STRAIGHT_G_PED_F)
//         
//         break;
//      case (EW_STRAIGHT_G)
//         
//         break;
//      case (EW_STRAIGHT_F)
//         
//         break;
//   }
//}

void AbstractController::tick()
{
   this->time--;
   std::cout << "Time: " << this->time << "\n";
   this->trigger();
}
