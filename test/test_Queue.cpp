#include <string>
#include <cstdio>
#include <unistd.h>
#include "../Queue.h"
#include <iostream>

/*used to test the queue*/
int main(void) {
    char* name = "/i1";

    /*create a queue and write several junk items to it. testing output in the
    queue (cerr) is used to confirm that messages are received as the queue
    creates a thread to poll for input*/
    Queue q(name, NULL);
    write_queue(name, 1, "Wayne", 0);
    write_queue(name, 2, "Simon", 0);
    write_queue(name, 3, "Tim", 0);
    write_queue(name, 4, "Barry", 0);
    sleep(10);
}
