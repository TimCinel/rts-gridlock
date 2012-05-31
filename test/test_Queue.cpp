#include <string>
#include <cstdio>
#include <unistd.h>
#include "../Queue.h"
#include <iostream>

int main(void) {
    char* name = "/i1";

    Queue q(name, NULL);
    write_queue(name, 1, "Wayne", 0);
    write_queue(name, 2, "Simon", 0);
    write_queue(name, 3, "Tim", 0);
    write_queue(name, 4, "Barry", 0);
    sleep(10);
}
