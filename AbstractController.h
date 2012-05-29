#ifndef ABSTRACTCONTROLLER
#define ABSTRACTCONTROLLER

class AbstractController
{
public:
    /*functions*/
    virtual void trigger() = 0;
    //unsigned int nextState();
    //unsigned int getState();
    void tick();

    inline void resetTimer(int time) { this->time = time; }
    inline int getTime() { return this->time; }

    virtual void setFlag(unsigned int flag) = 0;
    virtual int getFlag(unsigned int flag) = 0;

private:
    /*instance variables*/
    int s;
    int time;

};

#endif
