#ifndef ABSTRACTCONTROLLER
#define ABSTRACTCONTROLLER

class AbstractController
{
public:
    /*functions*/
    virtual void trigger() = 0;
    unsigned int nextState();
    unsigned int getState();
    void tick();

private:
    /*instance variables*/
    unsigned int s;
    unsigned int t;

};

#endif
