#ifndef INTERSECTIONCONTROLLER
#define INTERSECTIONCONTROLLER

#include "AbstractController.h"

#include "StateRecord.h"

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
      void[CONTROLLER_STATE_SENTINAL] *stateRecord;
      
};



#endif
