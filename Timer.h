#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>

// pulse contents
#define    CODE_TIMER    1

typedef struct  _pulse msg_header_t;

class Timer {

    private:
        int chanid;         //channel id
        int rcvid;          //process id of receiver

        /* timer materials. itimerspec struct is the key one, again pretty basic
         * since it'll just be 1 second */
        timer_t timerid;
        struct sigevent_t event;
        struct itimerspec timer;
        int connectid; // vale
    
    public:
        /* Constructor - as these are all going to be 1second clocks, can
         * probably just use an empty constuctor and the class values */
        Timer();
    
        /* run for a particular amount of time. "timeout" is a value in seconds.
        * After the timer has expired, send a pulse to the controller */
        int Run(int timeout);

        /* same as Run but with a timeout value of 1 second */
        int tick();

        /* (re)set the clock. Currently planned to just zero it, since I don't
         * think that telling the time is important, just keeping track of it */
        int SetTime();
}

#endif
