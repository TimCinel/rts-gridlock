#ifndef INTERSECTIONCONTROLLER
#define INTERSECTIONCONTROLLER

#include "AbstractController.h"

class IntersectionController : public AbstractController
{
   /*constructor*/
   public:
     IntersectionController();
      
   /*functions*/
      virtual void trigger();
      void display();


   /*instance variables*/
   private:
      /* a CONTROLLER_STATE_SENTINAL-element array of function pointers */
      void *(IntersectionController::*stateRecord[CONTROLLER_STATE_SENTINAL])();

  /*state functions */
        void *startup();
        void *ns_clear();
        void *ns_tram_g();
        void *ns_tram_f();
        void *ns_straight();
        void *ns_straight_g_ped_g();
        void *ns_straight_g_ped_f();
        void *ns_straight_g();
        void *ns_straight_f();
        void *ew_clear();
        void *ew_both_right_g();
        void *ew_both_right_f();
        void *ew_straight();
        void *ew_straight_g_ped_g();
        void *ew_straight_g_ped_f();
        void *ew_straight_g();
        void *ew_straight_f();
      
};



#endif
